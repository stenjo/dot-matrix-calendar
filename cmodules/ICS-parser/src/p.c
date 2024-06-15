#include "ics_parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>

// Helper function to find the end of the line ('\r\n')
static char *find_eol(char *s) {
    if (s == NULL) return NULL;
    while (*s != '\0' && !(s[0] == '\r' || s[0] == '\n')) {
        s++;
    }

    if (*s == '\0') {
        return NULL;  // Return NULL if we're at the end of the string
    } else {
        return s;
    }
}

// Helper function to check if a key exists within a line
int key_exists_within_line(char *line_start, char *line_end, const char *key)
{
    size_t key_length = strlen(key);
    // Ensure the key doesn't overrun the line boundaries
    if (line_start + key_length >= line_end) return 0;
    return strncmp(line_start, key, key_length) == 0;
}

bool startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

int parse_date_string(const char *date_str, struct tm *tm) {
    if (date_str == NULL) {
        return 0;
    }

    // Parse the date string.
    if (sscanf(date_str, "%4d%2d%2dT%2d%2d%2dZ",
               &tm->tm_year,
               &tm->tm_mon,
               &tm->tm_mday,
               &tm->tm_hour,
               &tm->tm_min,
               &tm->tm_sec) == 6) {
        tm->tm_year -= 1900; // tm_year is years since 1900
        tm->tm_mon -= 1;     // tm_mon is 0-based
        tm->tm_isdst = -1;   // Daylight saving information is unknown
        return 1; // Successfully parsed
    } else if (sscanf(date_str, "%4d%2d%2d",
               &tm->tm_year,
               &tm->tm_mon,
               &tm->tm_mday) == 3){

        tm->tm_year -= 1900; // tm_year is years since 1900
        tm->tm_mon -= 1;     // tm_mon is 0-based
        tm->tm_isdst = -1;   // Daylight saving information is unknown
        return 1; // Successfully parsed
    }
    else
    {
        return 0; // Failed to parse
    }
}

// Comparison function for qsort
int compare_events(const void *a, const void *b)
{
    const event_t *event_a = (const event_t *)a;
    const event_t *event_b = (const event_t *)b;

    // Use difftime to get a difference in seconds (double)
    double diff = difftime(event_a->tstart, event_b->tstart);
    if (diff < 0.0) {
        return -1; // a goes before b
    } else if (diff > 0.0) {
        return 1; // b goes before a
    } else {
        return 0; // a and b are equal
    }
}

// Function to sort the events in ics->events by their tstart field.
void sortEventsByStart(ics_t *ics)
{
    qsort(ics->events, ics->count, sizeof(event_t), compare_events);
}

event_t getEvent(const char *ics_data, const char **next) {
    event_t event = {NULL, NULL, NULL};

    *next = NULL;
    const char *key_vevent_start = "BEGIN:VEVENT";
    const char *key_vevent_end = "END:VEVENT";
    const char *key_summary = "SUMMARY:";
    const char *key_dtstart = "DTSTART:";
    const char *key_dtstart_date = "DTSTART;VALUE=DATE:";
    const char *key_dtend = "DTEND:";
    const char *key_dtend_date = "DTEND;VALUE=DATE:";
    
     // We will only parse the first VEVENT found
    char *vevent_start = strstr(ics_data, key_vevent_start);

    if (!vevent_start) {
        return event;
    }   // const char *key_vevent_end = "END:VEVENT";
    
    char *vevent_end = strstr(vevent_start, key_vevent_end);
    if (!vevent_end) {
        return event;  // Return if no VEVENT end is found
    }

    char *summary_start = strstr(vevent_start, key_summary);
    if (!summary_start || summary_start > vevent_end) {
        return event;
    }
    summary_start += strlen(key_summary);
    
    char *dtstart_start = strstr(vevent_start, key_dtstart);
    if (!dtstart_start || dtstart_start > vevent_end) {
        dtstart_start = strstr(vevent_start, key_dtstart_date);
        dtstart_start += strlen(key_dtstart_date);
        if (!dtstart_start || dtstart_start > vevent_end) {
            return event;
        }
    }
    else {
        dtstart_start += strlen(key_dtstart);
    }
   
    char *dtend_start = strstr(vevent_start, key_dtend);
     if (dtend_start && dtend_start < vevent_end) {
        dtend_start += strlen(key_dtend);
    }
    else {
        dtend_start = strstr(vevent_start, key_dtend_date);
        if (dtend_start && dtend_start < vevent_end) {
            dtend_start += strlen(key_dtend_date);
        }
        else {

        dtend_start = NULL;
        }
    }
   
    char *summary_end = find_eol(summary_start);
    char *dtstart_end = find_eol(dtstart_start);
    char *dtend_end = find_eol(dtend_start);
    if (!summary_end || summary_end > vevent_end || !dtstart_end || dtstart_end > vevent_end) {
        return event;
    }
    
    size_t summary_len = summary_end - summary_start;
    event.summary = strndup(summary_start, summary_len);
    
    size_t dtstart_len = dtstart_end - dtstart_start;
    // char *temp_dtstart = strndup(dtstart_start, dtstart_len);

    event.dtstart = strndup(dtstart_start, dtstart_len);

    if (dtend_start && dtend_end)
    {
        size_t dtend_len = dtend_end - dtend_start;
        event.dtend = strndup(dtend_start, dtend_len);
    }

    // Check if either allocation failed
    if (event.summary == NULL || event.dtstart == NULL ) {
        free(event.summary); // If one of the allocations failed, free the other one
        free(event.dtstart);
        if (event.dtend != NULL) {
            free(event.dtend);
        }
        event.summary = NULL;
        event.dtstart = NULL;
        event.dtend = NULL;
    }
    
    *next = vevent_end + strlen(key_vevent_end);

    return event;
}

size_t parse(ics_t *ics, const char *ics_data) 
{
    // Append the new data to the buffer
    size_t new_data_len = strlen(ics_data);
    size_t buffer_len = ics->buffer ? strlen(ics->buffer) : 0;
    char *new_buffer = realloc(ics->buffer, buffer_len + new_data_len + 1);
    if (!new_buffer) {
        // Handle memory allocation failure
        return ics->count;
    }
    ics->buffer = new_buffer;
    strcpy(ics->buffer + buffer_len, ics_data);

    // Parse the data in the buffer
    const char *next = ics->buffer;
    event_t event;
    while ((event = getEvent(next, &next)).dtstart != NULL) {
        struct tm tm_event_start;
        if (parse_date_string(event.dtstart, &tm_event_start)) {
            time_t event_time = mktime(&tm_event_start);

            if (
                (ics->startTime == 0 || difftime(event_time, ics->startTime) >= 0) &&
                (ics->endTime == 0 || difftime(ics->endTime, event_time) >= 0)
            )
            {
                ics->events[ics->count] = event;
                ics->count++;

                if (ics->count >= ics->capacity) {
                    size_t new_capacity = ics->capacity * 2;
                    event_t *new_events = realloc(ics->events, new_capacity * sizeof(event_t));
                    if (new_events) {
                        ics->events = new_events;
                        ics->capacity = new_capacity;
                    } else {
                        // Handle memory allocation failure
                        break;
                    }
                }
            } else {
                free(event.summary);
                free(event.dtstart);
                free(event.dtend);
            }
        } else {
            free(event.summary);
            free(event.dtstart);
            free(event.dtend);
        }
    }

    // Remove parsed data from the buffer
    if (next != ics->buffer) {
        size_t parsed_data_len = next - ics->buffer;
        memmove(ics->buffer, next, buffer_len + new_data_len - parsed_data_len + 1);
        char *shrunk_buffer = realloc(ics->buffer, strlen(ics->buffer) + 1);
        if (shrunk_buffer) {
            ics->buffer = shrunk_buffer;
        }
    }

    return ics->count;
}

void initIcs(ics_t *ics) {
    if (!ics) {
        return;
    }

    ics->count = 0;
    ics->capacity = 10;
    ics->events = malloc(ics->capacity * sizeof(event_t));
    ics->buffer = NULL;  // Initialize the buffer to NULL

    if (!ics->events) {
        ics->capacity = 0;
    }
}

void initIcsDates(ics_t *ics)
{
    if (!ics)
    {
        return;
    }

    // Initialize to zero, representing no specific time range
    ics->startTime = 0;
    ics->endTime = 0;
}

void cleanIcs(ics_t *ics) {
    if (!ics) {
        return;
    }

    for (size_t i = 0; i < ics->count; i++) {
        free(ics->events[i].summary);
        free(ics->events[i].dtstart);
        free(ics->events[i].dtend);
    }

    free(ics->events);
    ics->events = NULL;
    ics->count = 0;
    ics->capacity = 0;

    free(ics->buffer);  // Free the buffer
    ics->buffer = NULL;
}

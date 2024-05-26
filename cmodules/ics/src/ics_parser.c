#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "ics_parser.h"

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
    char *summary_start = strstr(vevent_start, key_summary);
    if (!summary_start|| summary_start > vevent_end) {
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

size_t parse(ics_t *ics, const char * ics_data) 
{
    // initIcs(ics);

    event_t event = getEvent(ics_data, &ics->next);

    while (ics->next != NULL && ics->count < MAX_EVENT_COUNT && event.dtstart != NULL)
    {
        struct tm tm_event_start;
        if (parse_date_string(event.dtstart, &tm_event_start)) {
            time_t event_time = mktime(&tm_event_start);

            if (
                (ics->startTime == 0 || difftime(event_time, ics->startTime) >= 0) &&
                (ics->endTime == 0 || difftime(ics->endTime, event_time) >= 0)
            )
            {
                struct tm tm_event_end;
                event.tstart = event_time;
                if (parse_date_string(event.dtend, &tm_event_end)) 
                {
                    time_t event_end_time = mktime(&tm_event_end);
                    event.tend = event_end_time;
                }
                ics->events[ics->count] = event;
                ics->count += 1;
            }
            else {
                free(event.summary);
                free(event.dtstart);
            }
        }
        event = getEvent(ics->next, &ics->next);
    }

    return ics->count;
}

event_t getFirstEvent(ics_t *ics)
{
    if (ics == NULL || ics->count == 0) {

        return (event_t){NULL, NULL}; 
    }

    ics->current = 0;

    return getCurrentEvent(ics);

}

event_t getNextEvent(ics_t *ics)
{
    if (atEnd(ics))
    {
        return (event_t){NULL, NULL}; 
    }

    ics->current++;
    return getCurrentEvent(ics);
}

event_t getLastEvent(ics_t *ics)
{
    if (ics == NULL || ics->count == 0) {
        return (event_t){NULL, NULL}; 
    }
    ics->current = ics->count - 1;
    return getCurrentEvent(ics);
}

event_t getCurrentEvent(ics_t *ics)
{
    if (ics == NULL || ics->count == 0) {
        return (event_t){NULL, NULL}; 
    }
    return ics->events[ics->current];
}

size_t setCurrentEvent(ics_t *ics, size_t index)
{
    if (ics == NULL || ics->count == 0 || index >= MAX_EVENT_COUNT || index >= ics->count) {
        printf("Error index outside boundaries: index: %zu, max: %d\n", index, MAX_EVENT_COUNT);
        return -1; 
    }
    ics->current = index;
    return ics->current;
}

event_t getEventAt(ics_t *ics, size_t index) 
{
    if (ics == NULL || ics->count == 0) {
        return (event_t){NULL, NULL}; 
    }
    size_t saved_index = ics->current;

    size_t current = setCurrentEvent(ics, index);
    if (current == -1) {
        return (event_t){NULL, NULL};
    }

    event_t event = getCurrentEvent(ics);
    ics->current = saved_index;

    return event;

}

time_t setStartDate(ics_t *ics, const char *start)
{
    struct tm tm_event_start;
    if (parse_date_string(start, &tm_event_start)) {
        time_t event_time = mktime(&tm_event_start);

        ics->startTime = event_time;
  
        return event_time;
    }
    printf("setTimeFailed with %s", start);
    return 0;
}

time_t setEndDate(ics_t *ics, const char *end)
{
    struct tm tm_event_start;
    if (parse_date_string(end, &tm_event_start)) {
        time_t event_time = mktime(&tm_event_start);

        ics->endTime = event_time;
    
        return event_time;
    }
    return 0;
}

void freeIcs(ics_t *ics)
{
    for (int i = 0; i < ics->count; i++) 
    {
        // Free dynamically allocated memory inside the event, if any.
        if (ics->events[i].summary != NULL) {
            free(ics->events[i].summary);
            ics->events[i].summary = NULL; // Avoid dangling pointers
        }
        if (ics->events[i].dtstart != NULL) {
            free(ics->events[i].dtstart);
            ics->events[i].dtstart = NULL; // Avoid dangling pointers
        }
        // If there are other dynamically allocated fields, also free them here.
    }
    initIcs(ics);
}

void initIcs(ics_t *ics)
{
    ics->current = 0;
    ics->count = 0;
    ics->next = NULL;
}

void initIcsDates(ics_t *ics)
{
    ics->startTime = 0;
    ics->endTime = 0;
}

bool atEnd(ics_t *ics)
{
    if (ics == NULL || ics->count == 0)
    { return true; }
    return ics->current >= (MAX_EVENT_COUNT - 1) || ics->current >= (ics->count - 1);
}
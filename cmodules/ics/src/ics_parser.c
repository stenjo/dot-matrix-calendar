#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "ics_parser.h"

// Helper function to find the end of the line ('\r\n')
static char *find_eol(char *s) {
    while (*s != '\0' && !(s[0] == '\r' || s[0] == '\n')) {
        s++;
    }

    if (*s == '\0') {
        return NULL;  // Return NULL if we're at the end of the string
    } else {
        return s;
    }
}

time_t parse_date_string(const char *date_str) {
    struct tm tm;
    strptime(date_str, "%Y%m%dT%H%M%SZ", &tm);
    time_t t = mktime(&tm);
    return t;
}

event_t getEvent(const char *ics_data, const char **next) {
    event_t event = {NULL, NULL};

    *next = NULL;
    const char *key_vevent_start = "BEGIN:VEVENT";
    const char *key_vevent_end = "END:VEVENT";
    const char *key_summary = "SUMMARY:";
    const char *key_dtstart = "DTSTART:";
    
     // We will only parse the first VEVENT found
    char *vevent_start = strstr(ics_data, key_vevent_start);

    if (!vevent_start) {
        return event;
    }   // const char *key_vevent_end = "END:VEVENT";
    
    char *vevent_end = strstr(vevent_start, key_vevent_end);
    char *summary_start = strstr(vevent_start, key_summary);
    if (!summary_start) {
        return event;
    }
    summary_start += strlen(key_summary);
    
    char *dtstart_start = strstr(vevent_start, key_dtstart);
     if (!dtstart_start) {
        return event;
    }
    dtstart_start += strlen(key_dtstart);
   
    char *summary_end = find_eol(summary_start);
    char *dtstart_end = find_eol(dtstart_start);
    if (!summary_end || !dtstart_end) {
        return event;
    }
    
    // Copy the strings to newly allocated memory
    size_t summary_len = summary_end - summary_start;
    event.summary = (char *)malloc(summary_len + 1);
    if (event.summary) {
        memcpy(event.summary, summary_start, summary_len);
        event.summary[summary_len] = '\0';
    }
    
   size_t dtstart_len = dtstart_end - dtstart_start;
    event.dtstart = (char *)malloc(dtstart_len + 1);
    if (event.dtstart) {
        memcpy(event.dtstart, dtstart_start, dtstart_len);
        event.dtstart[dtstart_len] = '\0';
    }

    // Check if either allocation failed
    if (event.summary == NULL || event.dtstart == NULL) {
        free(event.summary); // If one of the allocations failed, free the other one
        free(event.dtstart);
        event.summary = NULL;
        event.dtstart = NULL;
    }
    
    *next = vevent_end;

    return event;
}

size_t parse(ics_t *ics, const char * ics_data) 
{
    initIcs(ics);

    event_t event = getEvent(ics_data, &ics->next);

    while (ics->next != NULL && ics->count < MAX_EVENT_COUNT)
    {
        ics->events[ics->count] = event;
        ics->count += 1;
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
    if (ics == NULL || ics->count == 0 || index >= MAX_EVENT_COUNT || index >= ics->count || index < 0) {
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

event_t getNextEventInRange(ics_t *ics, const char *start_date_str, const char *end_date_str) {
    // Check if the start date string is supplied
    if (start_date_str == NULL) {
        // Handle error or return an empty event.
        return (event_t){NULL, NULL};
    }

    // Convert date strings to time_t for comparison.
    time_t start = parse_date_string(start_date_str);
    time_t end;
    // If the end date string is not supplied or is empty, set it to the maximum time_t value.
    if (end_date_str == NULL || strlen(end_date_str) == 0) {
        end = 0;
    } else {
        end = parse_date_string(end_date_str);
    }

    while (!atEnd(ics)) {
        ics->current++;
        event_t event = getCurrentEvent(ics);
        
        // Parse the event's start date.
        time_t event_start = parse_date_string(event.dtstart);
        
        // Check if the event's start date is within the range.
        if (difftime(event_start, start) >= 0 && ( end == 0 || difftime(event_start, end) <= 0)) {
            return event; // Event is within the range.
        }
    }

    // Return an empty event if no more events are within the range.
    return (event_t){NULL, NULL};
}


time_t setStartDate(ics_t *ics, const char *start)
{
    time_t event_start = parse_date_string(start);
    ics->startTime = event_start;
  
    return event_start;
}

void initIcs(ics_t *ics)
{
    ics->current = 0;
    ics->count = 0;
    ics->next = NULL;
    ics->startTime = 0;
    ics->endTime = 0;
    memset(ics->events, 0, sizeof(ics->events));
}

bool atEnd(ics_t *ics)
{
    if (ics == NULL || ics->count == 0)
    { return true; }
    return ics->current >= (MAX_EVENT_COUNT - 1) || ics->current >= (ics->count - 1);
}
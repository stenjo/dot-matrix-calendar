#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

ics_t parse(const char * ics_data) 
{
    ics_t ics;
    initIcs(&ics);

    event_t event = getEvent(ics_data, &ics.next);

    while (ics.next != NULL && ics.count < MAX_EVENT_COUNT)
    {
        ics.events[ics.count] = event;
        ics.count += 1;
        event = getEvent(ics.next, &ics.next);
    }

    return ics;
}

event_t getFirstEvent(ics_t *ics)
{
    if (ics == NULL || ics->count == 0) {
        return (event_t){NULL, NULL}; 
    }

    ics->current = 0;

    return ics->events[0];

}

event_t getNextEvent(ics_t *ics)
{
    if (ics->current < MAX_EVENT_COUNT - 1 && ics->current < ics->count -1)
    {
        ics->current++;
        return ics->events[ics->current];
    }

    return (event_t){NULL, NULL}; 

}

event_t getLastEvent(ics_t *ics)
{
    if (ics == NULL || ics->count == 0) {
        return (event_t){NULL, NULL}; 
    }
    ics->current = ics->count - 1;
    return ics->events[ics->current];
}

void initIcs(ics_t *ics)
{
    ics->current = 0;
    ics->count = 0;
    ics->next = NULL;
    memset(ics->events, 0, sizeof(ics->events));
}
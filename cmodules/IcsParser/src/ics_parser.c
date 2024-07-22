#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>

#include "ics_utils.h"
#include "ics_event.h"
#include "ics_parser.h"

#define CHUNK_SIZE 1024


// Statics
static event_t * createRepeatEvent(event_t * event, time_t interval) {
    event_t * revent = cloneEvent(event);
    
    time_t duration = revent->tend - revent->tstart;
    revent->tstart = revent->tstart + interval;
    revent->tend = revent->tstart + duration;
    updateDateStr(revent->dtstart, revent->tstart);
    updateDateStr(revent->dtend, revent->tend);

    return revent;

}

static size_t addEvent(ics_t *ics, event_t *event) {
    if (ics->count >= (int)ics->capacity) {
        size_t new_capacity = ics->capacity + 20;
        event_t **new_events = realloc(ics->events, new_capacity * sizeof(event_t *));
        if (new_events) {
            ics->events = new_events;
            ics->capacity = new_capacity;
        } else {
            printf("Memory allocation failed\n");
            return (size_t)-1;
        }
    }

    ics->events[ics->count] = event;
    ics->count++;
    return ics->count;
}

static time_t handleRRule(event_t *event, time_t filter_time) {
    if (filter_time == 0) {
        return 0; // TODO: handle no filtertime
    }
    if (event->rrule && strstr(event->rrule, "WEEKLY")) {
        time_t interval_seconds = atoi(event->interval) * 7 * 3600 * 24; // seconds
        time_t event_age = filter_time - event->tstart; // timestamp
        time_t repeats = (event_age / interval_seconds) +1;
        time_t delta = interval_seconds * repeats;
        event->tstart = event->tstart + delta;
        event->tend = event->tend + delta;
        updateDateStr(event->dtstart, event->tstart);
        updateDateStr(event->dtend, event->tend);

        return interval_seconds;
    }
    return 0;
}

// Globals

size_t parseFile(ics_t *ics, const char *file_path) {
    resetGetEvent(); // Start off with clear buffer
    FILE *f = fopen(file_path, "rb");  // Open file for reading
    if (!f) {
        // Print an error message if the file could not be opened
        printf("Error opening file '%s': %d\n", file_path, errno);
        return 0;
    }

    // Allocate a buffer for reading chunks
    char *buffer = malloc(CHUNK_SIZE + 1);  // +1 for null-termination
    if (!buffer) {
        fclose(f);
        printf("Error allocating memory\n");
        return 0;
    }

    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, CHUNK_SIZE, f)) > 0) {
        buffer[bytes_read] = '\0';  // Null-terminate the buffer
        parseIcs(ics, buffer);  // Pass chunk to parseIcs
    }

    // Check for read errors
    if (ferror(f)) {
        printf("Error reading file '%s'\n", file_path);
    }

    // Clean up
    free(buffer);
    fclose(f);

    resetGetEvent(); // Reset
    return ics->count;
}

size_t parseIcs(ics_t *ics, const char *ics_data) {
    updateBuffer(ics_data);
    event_t * event;
    while ((event = getEvent()) != NULL && event->dtstart != NULL) {
        if (event->tstart < 0) { freeEvent(event); continue; }

        // Handle RRule
        time_t interval_seconds = handleRRule(event, ics->startTime);


        double afterStartFilter = difftime(event->tstart, ics->startTime);
        double beforeEndFilter = difftime(ics->endTime, event->tstart);
        if (event->tend > event->tstart) {
            beforeEndFilter = difftime(ics->endTime, event->tend);
        }

        if ((ics->startTime != 0 && afterStartFilter < 0) || (ics->endTime != 0 && beforeEndFilter < 0)) { 
            freeEvent(event); 
            continue;
        }

        addEvent(ics, event);

        while (interval_seconds > 0 && ics->endTime > 0 && difftime(ics->endTime, event->tstart + interval_seconds) >= 0) {
            event = createRepeatEvent(event, interval_seconds);
            if (event != NULL) addEvent(ics, event);
        }

    }

    return ics->count;
}


time_t setStartDate(ics_t *ics, const char *start)
{
    struct tm tm_event_start = { 0};
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
    struct tm tm_event_end = { 0 };
    if (parse_date_string(end, &tm_event_end)) {
        time_t event_time = mktime(&tm_event_end);

        ics->endTime = event_time;
    
        return event_time;
    }
    return 0;
}

void initIcs(ics_t *ics) {
    ics->events = malloc(10 * sizeof(event_t *));
    ics->count = 0;
    ics->capacity = 10;
    ics->startTime = 0;
    ics->endTime = 0;
}

void freeIcs(ics_t *ics) {
    if (ics->events == NULL) return;
    for (int i = 0; i < ics->count; i++) {
        if(ics->events[i] != NULL) {
            freeEvent(ics->events[i]);
            ics->events[i] = NULL;
        }
    }
    free(ics->events);
    ics->events = NULL;
    ics->count = 0;
    ics->events = NULL;
    resetGetEvent();
}

void initIcsDates(ics_t *ics)
{
    ics->startTime = 0;
    ics->endTime = 0;
}

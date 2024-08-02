#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <esp_log.h>

#include "ics_utils.h"
#include "ics_event.h"
#include "ics_parser.h"

#define CHUNK_SIZE 1024


// Statics


static event_t * createRepeatEvent(event_t * event, time_t interval) {
    event_t * r_event = cloneEvent(event);
    
    time_t duration = r_event->tend - r_event->tstart;
    r_event->tstart = r_event->tstart + interval;
    r_event->tend = r_event->tstart + duration;
    updateDateStr(r_event->dtstart, r_event->tstart);
    updateDateStr(r_event->dtend, r_event->tend);

    return r_event;

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

    if (filter_time == 0) return 0; // TODO: handle no filtertime
    if (event->rrule == NULL || event->rrule[0] == '\0') return 0;

    time_t interval = 1;
    if (event->interval != NULL && event->interval[0] != '\0') interval = atoi(event->interval);

    ESP_LOGI("DEBUG", "REvent: %s\n", event->summary);
    if (strstr(event->rrule, "WEEKLY")) {

        time_t interval_seconds = interval * 7 * 3600 * 24; // seconds
        time_t filter_time_midnight = getMidnightTimeStamp(filter_time);
        time_t event_age = filter_time_midnight - event->tstart; // timestamp
        time_t repeats = (event_age / interval_seconds) +1;
        time_t delta = interval_seconds * repeats;
        event->tstart = event->tstart + delta;
        event->tend = event->tend + delta;
        updateDateStr(event->dtstart, event->tstart);
        updateDateStr(event->dtend, event->tend);
        if (event->tstart <= filter_time && event->tstart > filter_time - 86400*2) {
            char str[256] = "00000000000000000000";
            ESP_LOGI("DEBUG", "  filter midnight: %ld %s\n", (long)filter_time_midnight, updateDateStr(str, filter_time_midnight));
            ESP_LOGI("DEBUG", "  filter_time:     %ld %s\n", (long)filter_time, updateDateStr(str, filter_time));
            ESP_LOGI("DEBUG", "  event start:     %ld %s\n\n", (long)event->tstart, updateDateStr(str, event->tstart));
        }

        return interval_seconds;
    }
    if (strstr(event->rrule, "YEARLY")) {
        struct tm tm_event_start = { 0};
        time_t duration = event->tend - event->tstart;
        if (parse_date_string(event->dtstart, &tm_event_start)) {
            tm_event_start.tm_year = getYearFromTime(filter_time) - 1900;
            time_t event_start_time = mktime(&tm_event_start);
            event->tstart = event_start_time;
            event->tend = event_start_time + duration;
            updateDateStr(event->dtstart, event->tstart);
            updateDateStr(event->dtend, event->tend);
        }
        time_t interval_seconds = interval * 365 * 3600 * 24; // seconds
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
        if (event->tstart < 0) { 
            freeEvent(event); 
            continue; 
        }

        // Handle RRule
        time_t interval_seconds = handleRRule(event, ics->startTime);

        double afterStartFilter = difftime(event->tstart, getMidnightTimeStamp(ics->startTime));
        // if (event->tend > event->tstart) {
        //     afterStartFilter = difftime(event->tend, ics->startTime);
        // }
        double beforeEndFilter = difftime(ics->endTime, event->tstart);

        if ((ics->startTime != 0 && afterStartFilter < 0) || (ics->endTime != 0 && beforeEndFilter < 0)) { 
            freeEvent(event); 
            continue;
        }

        addEvent(ics, event);

        while (interval_seconds > 0 && ics->endTime > 0 && difftime(ics->endTime, event->tstart + interval_seconds) >= 0) {
            event = createRepeatEvent(event, interval_seconds);
            if (event != NULL) { addEvent(ics, event); }
        }
    }
    return ics->count;
}


time_t setStartDate(ics_t *ics, const char *start)
{
    ics->startTime = getTimeStamp(start);
    if (ics->startTime < 0) { 
        printf("setTimeFailed with %s", start);
        return 0;
    }
    return ics->startTime;
}

time_t setEndDate(ics_t *ics, const char *end)
{
    ics->endTime = getTimeStamp(end);
    if (ics->endTime < 0) {
        printf("setTimeFailed with %s", end);
        return 0;
    }
    return ics->endTime;
}

void initIcs(ics_t *ics) {
    ics->events = calloc(10, sizeof(event_t *));
    ics->count = 0;
    ics->capacity = 10;
    ics->startTime = 0;
    ics->endTime = 0;
}

void freeIcs(ics_t *ics) {
    for (int i = 0; i < ics->count; i++) {
        if (ics->events[i] != NULL) {
            freeEvent(ics->events[i]);
            ics->events[i] = NULL;
        }
    }
    free(ics->events);  // Free the array of event pointers
    ics->count = 0;
    resetGetEvent();
}

void initIcsDates(ics_t *ics)
{
    ics->startTime = 0;
    ics->endTime = 0;
}

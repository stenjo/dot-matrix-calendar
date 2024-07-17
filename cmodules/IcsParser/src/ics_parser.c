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
    event_t event;
    while ((event = getEvent()).dtstart != NULL) {

        struct tm tm_event_start = {0};
        if (parse_date_string(event.dtstart, &tm_event_start)) {
            time_t event_time = mktime(&tm_event_start);
            event.tstart = event_time;

            double afterStartFilter = difftime(event_time, ics->startTime);
            double beforeEndFilter = difftime(ics->endTime, event_time);

            if (
                (ics->startTime == 0 || afterStartFilter >= 0) &&
                (ics->endTime == 0 || beforeEndFilter >= 0)
            ) {
                if (ics->count >= ics->capacity) {
                    size_t new_capacity = ics->capacity + 20;
                    event_t *new_events = realloc(ics->events, new_capacity * sizeof(event_t));
                    if (new_events) {
                        ics->events = new_events;
                        ics->capacity = new_capacity;
                    } else {
                        printf("Memory allocation failed\n");
                        break;
                    }
                }
                memcpy(&(ics->events[ics->count]), &event, sizeof(event));
                ics->count++;
            } else {
                freeEvent(event);
            }
        } else {
            freeEvent(event);
        }
    }

    return ics->count;
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

void initIcs(ics_t *ics) {
    ics->events = malloc(10 * sizeof(event_t));
    ics->count = 0;
    ics->capacity = 10;
    ics->startTime = 0;
    ics->endTime = 0;
}

void freeIcs(ics_t *ics) {
    for (size_t i = 0; i < ics->count; i++) {
        freeEvent(ics->events[i]);
    }
    free(ics->events);
    ics->count = 0;
    ics->events = NULL;
    resetGetEvent();
}

void initIcsDates(ics_t *ics)
{
    ics->startTime = 0;
    ics->endTime = 0;
}

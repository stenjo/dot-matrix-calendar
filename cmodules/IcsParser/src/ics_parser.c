#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>

#include "ics_utils.h"
#include "ics_event.h"
#include "ics_parser.h"


// Helper function to find the end of the line ('\r\n')
// static char *find_eol(char *s) {
//     if (s == NULL) return NULL;
//     while (*s != '\0' && !(s[0] == '\r' || s[0] == '\n')) {
//         s++;
//     }

//     if (*s == '\0') {
//         return NULL;  // Return NULL if we're at the end of the string
//     } else {
//         return s;
//     }
// }

size_t parseFile(ics_t * ics, const char *file_path) {
    // Allocate a buffer for the file's contents
    char *buffer = NULL;
    // initIcs(ics);
    long length;
    FILE *f = fopen(file_path, "rb");  // Open file for reading

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length + 1);

        if (buffer) {
            fread(buffer, 1, length, f);
            buffer[length] = '\0';  // Null-terminate the buffer
        }
        fclose(f);

        if (buffer) {
            // Parse the ICS data from the buffer
            parseIcs(ics, buffer);
            free(buffer);  // Don't forget to free the allocated buffer
        }
     } else {
        // Print an error message if the file could not be opened
        printf("Error opening file '%s': %d\n", file_path, errno);
    }

    return ics->count;
}


size_t parseIcs(ics_t *ics, const char *ics_data) {

    updateBuffer(ics_data);

    event_t event;
    while ((event = getEvent()).dtstart != NULL) {
        struct tm tm_event_start;
        // struct tm tm_event_end;
        if (parse_date_string(event.dtstart, &tm_event_start)) {
            time_t event_time = mktime(&tm_event_start);
            event.tstart = event_time;

            double afterStartFilter = difftime(event_time, ics->startTime);
            double beforeEndFilter = difftime(ics->endTime, event_time);

            if (
                (ics->startTime == 0 || afterStartFilter >= 0) &&
                (ics->endTime == 0 || beforeEndFilter >= 0)
            ) {
                parse_date_string(event.dtstart, &tm_event_start);
                // time_t event_time_end = mktime(&tm_event_end);
                // event.tend = event_time;
                if (ics->count >= ics->capacity) {
                    size_t new_capacity = ics->capacity * 2;
                    event_t *new_events = realloc(ics->events, new_capacity * sizeof(event_t));
                    if (new_events) {
                        ics->events = new_events;
                        ics->capacity = new_capacity;
                    } else {
                        printf("Memory allocation failed\n");
                        break;
                    }
                }
                ics->events[ics->count] = event;
                ics->count++;
            } else {
                freeEvent(event);
            }
        } else {
            freeEvent(event);
        }
    }

    // printf("Parsed %zu events\n", ics->count);
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
    ics->buffer = NULL;
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
    // free(ics->events);
    free(ics->buffer);
}

void initIcsDates(ics_t *ics)
{
    ics->startTime = 0;
    ics->endTime = 0;
}

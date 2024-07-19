#include "ics_event.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ics_parser.h"
#include "ics_utils.h"

const event_t nullEvent = {NULL, NULL, NULL, NULL, 0,0};
static char *data_buffer = NULL;

void updateBuffer(const char *new_data) {

    if (data_buffer == NULL) {
        // Allocate initial buffer if data_buffer is NULL
        data_buffer = malloc(strlen(new_data) + 1);
        memset(data_buffer, 0xFF, strlen(new_data) + 1);
        if (data_buffer == NULL) {
            printf("\nMemory allocation failed, trying to allocate %zu bytes\n", (size_t)(strlen(new_data) + 1));
            return;
        }
        strcpy(data_buffer, new_data);
        data_buffer[strlen(new_data)] = '\0';
    } else {
        // Reallocate buffer if data_buffer is not NULL
        size_t new_data_len = strlen(new_data);
        size_t buffer_len = strlen(data_buffer);
        char *new_buffer = realloc(data_buffer, buffer_len + new_data_len + 1);
        if (!new_buffer) {
            printf("Memory reallocation failed, trying to reallocate %zu bytes with additional %zu bytes\n", buffer_len, new_data_len);
            return;
        }
        data_buffer = new_buffer;
        memset(data_buffer+buffer_len, 0, new_data_len + 1);
        strcat(data_buffer, new_data);
    
    }

}

void resetGetEvent(void) {
    free(data_buffer);
    data_buffer = NULL;
}

event_t getEvent(void) {
    event_t event = nullEvent;

    if (data_buffer == NULL || *data_buffer == '\0') {
        return event;
    }

    const char *vevent_start = strstr(data_buffer, "BEGIN:VEVENT");
    if (!vevent_start) {
        return event;
    }

    const char *vevent_end = strstr(vevent_start, "END:VEVENT");
    if (!vevent_end) {
        return event;
    }

    event.summary = extract_property(vevent_start, "SUMMARY:", vevent_end);

    event.uid = extract_property(vevent_start, "UID:", vevent_end);
    event.dtstart = extract_property(vevent_start, "DTSTART;VALUE=DATE:", vevent_end);
    if (!event.dtstart) {
        event.dtstart = extract_property(vevent_start, "DTSTART:", vevent_end);
    }

    event.dtend = extract_property(vevent_start, "DTEND;VALUE=DATE:", vevent_end);
    if (!event.dtend) {
        event.dtend = extract_property(vevent_start, "DTEND:", vevent_end);
    }

    event.rrule = extract_property(vevent_start, "RRULE:FREQ=", vevent_end);
    event.interval = extract_property(vevent_start, "INTERVAL=", vevent_end);

    size_t offset = (vevent_end - data_buffer) + strlen("END:VEVENT");
    char * next_vevent_start = strstr(data_buffer + offset, "BEGIN:VEVENT");
    if (next_vevent_start) {
        offset = next_vevent_start - data_buffer;
    }
    if (offset >= strlen(data_buffer)) {
        free(data_buffer);
        data_buffer = NULL;
    } else {
        size_t buffer_length = strlen(data_buffer) - offset;
        memmove(data_buffer, data_buffer + offset, buffer_length + 1);
        char *new_buffer = realloc(data_buffer, buffer_length + 1);
        if (!new_buffer) {
            printf("Memory reallocation failed, trying to reallocate %zu bytes\n", buffer_length);
            return event;
        }
        data_buffer = new_buffer;
    }
    return event;

}

char *extract_property(const char *data, const char *property, const char *end) {
    const char *start = strnstr(data, property, end - data);
    if (!start) {
        return NULL;
    }

    start += strlen(property);

    // Find the end of the property value (handle both \r\n and \n)
    const char *terminator = strstr(start, "\r\n");

    if (!terminator) terminator = strstr(start, "\n");
    if (!terminator) return NULL;

    size_t len = terminator - start;
    char *value = malloc(len + 1);
    if (value) {
        strncpy(value, start, len);
        value[len] = '\0';
    }
    return value;
}

event_t getFirstEvent(ics_t *ics)
{
    if (ics == NULL || ics->count == 0) return nullEvent; 
    ics->current = 0;
    return getCurrentEvent(ics);

}

event_t getNextEvent(ics_t *ics)
{
    if (atEnd(ics)) return nullEvent; 
    ics->current++;
    return getCurrentEvent(ics);
}

event_t getLastEvent(ics_t *ics)
{
    if (ics == NULL || ics->count == 0) return nullEvent; 
    ics->current = ics->count - 1;
    return getCurrentEvent(ics);
}

event_t getCurrentEvent(ics_t *ics)
{
    if (ics == NULL || ics->count == 0) return nullEvent; 
    return ics->events[ics->current];
}

int setCurrentEvent(ics_t *ics, size_t index)
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
    if (ics == NULL || ics->count == 0) return nullEvent; 

    size_t saved_index = ics->current;
    int current = setCurrentEvent(ics, index);
    if (current == -1) {
        return nullEvent;
    }

    event_t event = getCurrentEvent(ics);
    ics->current = saved_index;

    return event;

}

// Comparison function for qsort
static int compare_events(const void *a, const void *b)
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


bool atEnd(ics_t *ics) {
    if (ics == NULL || ics->count == 0) {
        return true;
    }
    return ics->current >= (ics->capacity - 1) || ics->current >= (ics->count - 1);
}

void freeEvent(event_t event) {
    if (event.summary != NULL) {
        free(event.summary);
        event.summary = NULL;
    }
    if (event.dtstart != NULL) {
        free(event.dtstart);
        event.dtstart = NULL;
    };

    if (event.dtend != NULL) {
        free(event.dtend);
        event.dtend = NULL;
    }

    if (event.uid != NULL) {
        free(event.uid);
        event.uid = NULL;
    }
}


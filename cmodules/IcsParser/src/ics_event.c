#include "ics_event.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ics_parser.h"


static char *next = NULL;

void updateBuffer(const char *new_data) {
    if (next == NULL) {
        // Allocate initial buffer if next is NULL
        next = malloc(strlen(new_data) + 1);
        if (next == NULL) {
            printf("Memory allocation failed\n");
            return;
        }
        strcpy(next, new_data);
    } else {
        // Reallocate buffer if next is not NULL
        size_t new_data_len = strlen(new_data);
        size_t buffer_len = strlen(next);
        char *new_buffer = realloc(next, buffer_len + new_data_len + 1);
        if (!new_buffer) {
            printf("Memory allocation failed\n");
            return;
        }
        next = new_buffer;
        strcat(next, new_data);
    }
}

void resetGetEvent(void) {
    free(next);
    next = NULL;
}

event_t getEvent(void) {
    event_t event = {NULL, NULL, NULL, 0, 0};

    if (next == NULL || *next == '\0') {
        return event;
    }

    const char *vevent_start = strstr(next, "BEGIN:VEVENT");
    if (!vevent_start) {
        free(next);
        next = NULL;
        return event;
    }

    const char *vevent_end = strstr(vevent_start, "END:VEVENT");
    if (!vevent_end) {
        return event;
    }

    event.summary = extract_property(vevent_start, "SUMMARY:", vevent_end);
    event.dtstart = extract_property(vevent_start, "DTSTART;VALUE=DATE:", vevent_end);
    if (!event.dtstart) {
        event.dtstart = extract_property(vevent_start, "DTSTART:", vevent_end);
    }

    event.dtend = extract_property(vevent_start, "DTEND;VALUE=DATE:", vevent_end);
    if (!event.dtend) {
        event.dtend = extract_property(vevent_start, "DTEND:", vevent_end);
    }

    size_t offset = (vevent_end - next) + strlen("END:VEVENT") + 2;
    if (offset >= strlen(next)) {
        free(next);
        next = NULL;
    } else {
        memmove(next, next + offset, strlen(next) - offset + 1);
    }

    return event;
}

char *extract_property(const char *data, const char *property, const char *end) {
    char * src = strndup(data, end - data);
    const char *start = strstr(src, property);
    if (!start) {
        return NULL;
    }

    start += strlen(property);

    // Find the end of the property value (handle both \r\n and \n)
    const char *terminator = strstr(start, "\r\n");
    if (!terminator) {
        terminator = strstr(start, "\n");
    }

    if (!terminator) {
        return NULL;
    }

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
    if (ics == NULL || ics->count == 0) {

        return (event_t){NULL, NULL, NULL,0,0}; 
    }

    ics->current = 0;

    return getCurrentEvent(ics);

}

event_t getNextEvent(ics_t *ics)
{
    if (atEnd(ics))
    {
        return (event_t){NULL, NULL, NULL,0,0}; 
    }

    ics->current++;
    return getCurrentEvent(ics);
}

event_t getLastEvent(ics_t *ics)
{
    if (ics == NULL || ics->count == 0) {
        return (event_t){NULL, NULL, NULL,0,0}; 
    }
    ics->current = ics->count - 1;
    return getCurrentEvent(ics);
}

event_t getCurrentEvent(ics_t *ics)
{
    if (ics == NULL || ics->count == 0) {
        return (event_t){NULL, NULL, NULL,0,0}; 
    }
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
    if (ics == NULL || ics->count == 0) {
        return (event_t){NULL, NULL, NULL,0,0}; 
    }
    size_t saved_index = ics->current;

    int current = setCurrentEvent(ics, index);
    if (current == -1) {
        return (event_t){NULL, NULL, NULL,0,0};
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
    free(event.summary);
    free(event.dtstart);
    free(event.dtend);
}


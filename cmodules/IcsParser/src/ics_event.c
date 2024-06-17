#include "ics_event.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char* extract_property(const char *data, const char *property) {
    char *property_start = strstr(data, property);
    if (!property_start) {
        return NULL;
    }

    // Move past the property name
    property_start += strlen(property);

    // Find the end of the property line
    char *property_end = strstr(property_start, "\r\n");
    if (!property_end) {
        return NULL;
    }

    // Calculate the length of the property value
    size_t property_length = property_end - property_start;

    // Allocate memory for the property value
    char *property_value = (char*)malloc(property_length + 1);
    if (!property_value) {
        return NULL;
    }

    // Copy the property value
    strncpy(property_value, property_start, property_length);
    property_value[property_length] = '\0';

    return property_value;
}

event_t getEvent(const char *data, const char **next) {
    event_t event = {NULL, NULL, NULL, 0, 0};
    const char *vevent_start = strstr(data, "BEGIN:VEVENT");
    if (!vevent_start) {
        *next = NULL;
        return event;
    }

    const char *vevent_end = strstr(vevent_start, "END:VEVENT");
    if (!vevent_end) {
        *next = vevent_start;
        return event;
    }

    event.summary = extract_property(vevent_start, "SUMMARY:");
    event.dtstart = extract_property(vevent_start, "DTSTART:");
    event.dtend = extract_property(vevent_start, "DTEND:");

    if (!event.summary || !event.dtstart || !event.dtend) {
        free(event.summary);
        free(event.dtstart);
        free(event.dtend);
        event.summary = NULL;
        event.dtstart = NULL;
        event.dtend = NULL;
    }

    *next = vevent_end + strlen("END:VEVENT") + 2;
    if (**next == '\0') {
        *next = NULL;
    }

    return event;
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


bool atEnd(ics_t *ics)
{
    if (ics == NULL || ics->count == 0)
    { return true; }
    return ics->current >= (MAX_EVENT_COUNT - 1) || ics->current >= (ics->count - 1);
}
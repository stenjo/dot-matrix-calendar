#include "ics_event.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ics_parser.h"
#include "ics_utils.h"

typedef struct {
    size_t length;
    char *buffer;
} buffer_t;


static buffer_t data={0,NULL};

void updateBuffer(const char *new_data) {

    size_t new_data_len = strlen(new_data);
    if (data.buffer == NULL) {
        // Allocate initial buffer if data_buffer is NULL
        data.buffer = calloc(new_data_len + 1, sizeof(char));
        if (data.buffer == NULL) {
            printf("\nMemory allocation failed, trying to allocate %zu bytes\n", (size_t)(strlen(new_data) + 1));
            return;
        }
        strcpy(data.buffer, new_data);
        data.length = new_data_len + 1;
    } else {
        // Reallocate buffer if data_buffer is not NULL
        size_t new_length = data.length + new_data_len;
        // if (data.length != strlen(data.buffer)+1) { printf("%d: diff %zu %zu\n", __LINE__, data.length, strlen(data.buffer)+1);}
        char *new_buffer = realloc(data.buffer, new_length);
        if (!new_buffer) {
            printf("Memory reallocation failed, trying to reallocate %zu bytes with additional %zu bytes\n", data.length, new_data_len);
            return;
        }
        data.buffer = new_buffer;
        strcat(data.buffer, new_data);
        data.length = new_length;
        // if (data.length != strlen(data.buffer)+1) { printf("%d: diff %zu %zu\n", __LINE__, data.length, strlen(data.buffer)+1);}
    }

}

void resetGetEvent(void) {
    free(data.buffer);
    data.buffer = NULL;
    data.length = 0;
}

event_t * getEvent(void) {

    if (data.buffer == NULL || *data.buffer == '\0') {
        return NULL;
    }

    const char *vevent_start = strstr(data.buffer, "BEGIN:VEVENT");
    if (!vevent_start) {
        return NULL;
    }

    const char *vevent_end = strstr(vevent_start, "END:VEVENT");
    if (!vevent_end) {
        return NULL;
    }

    // Allocate event in memory. Freed in parser
    event_t * event = calloc(1,sizeof(event_t));

    event->summary = extract_property(vevent_start, "SUMMARY:", vevent_end);
    event->uid = extract_property(vevent_start, "UID:", vevent_end);
    event->dtstart = extract_property(vevent_start, "DTSTART;VALUE=DATE:", vevent_end);
    if (!event->dtstart) {
        event->dtstart = extract_property(vevent_start, "DTSTART:", vevent_end);
    }

    event->dtend = extract_property(vevent_start, "DTEND;VALUE=DATE:", vevent_end);
    if (!event->dtend) {
        event->dtend = extract_property(vevent_start, "DTEND:", vevent_end);
    }

    event->rrule = extract_property(vevent_start, "RRULE:FREQ=", vevent_end);
    if (event->rrule) {
        char const * semi = strstr(event->rrule, ";");
        if (semi) {
            size_t rrule_length = semi - event->rrule;
            event->rrule = realloc(event->rrule, rrule_length);
            if (!event->rrule) {
                printf("Memory reallocation failed, trying to reallocate %zu bytes\n", rrule_length);
                return event;
            }
            event->rrule[rrule_length] = '\0';
        }
    }
    event->interval = extract_property(vevent_start, "INTERVAL=", vevent_end);

    size_t offset = (vevent_end - data.buffer) + strlen("END:VEVENT");
    char const * next_vevent_start = strstr(data.buffer + offset, "BEGIN:VEVENT");
    if (next_vevent_start) {
        offset = next_vevent_start - data.buffer;
    }
    if (data.length != strlen(data.buffer)+1) { printf("%d: diff %zu %zu\n", __LINE__, data.length, strlen(data.buffer)+1);}
    if (offset >= strlen(data.buffer)) {
        free(data.buffer);
        data.buffer = NULL;
        data.length = 0;
        // printf("Buffer freed\n");
    } else {
        size_t buffer_length = data.length - offset;
        memmove(data.buffer, data.buffer + offset, buffer_length);
        char *new_buffer = realloc(data.buffer, buffer_length);
        // printf("Buffer realloc, %zu bytes\n", buffer_length);

        if (!new_buffer) {
            printf("Memory reallocation failed, trying to reallocate %zu bytes\n", buffer_length);
            return event;
        }
        data.buffer = new_buffer;
        data.length = buffer_length;
    }

    // Update tstart and tend
    event->tstart = getTimeStamp(event->dtstart);
    event->tend = getTimeStamp(event->dtend);

    return event;

}

char *extract_property(const char *source, const char *property, const char *end) {
    const char *start = strnstr(source, property, end - source);
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
    nukeChar(value, '\\');
    return value;
}

event_t *getFirstEvent(ics_t *ics)
{
    if (ics == NULL || ics->count == 0) return NULL; 
    ics->current = 0;
    return getCurrentEvent(ics);

}

event_t *getNextEvent(ics_t *ics)
{
    if (atEnd(ics)) return NULL; 
    ics->current++;
    return getCurrentEvent(ics);
}

event_t *getLastEvent(ics_t *ics)
{
    if (ics == NULL || ics->count == 0) return NULL; 
    ics->current = ics->count - 1;
    return getCurrentEvent(ics);
}

event_t *getCurrentEvent(const ics_t *ics)
{
    if (ics == NULL || ics->count == 0) return NULL; 
    return ics->events[ics->current];
}

int setCurrentEvent(ics_t *ics, int index)
{
    if (ics == NULL || ics->count == 0 || index >= MAX_EVENT_COUNT || index >= ics->count) {
        printf("Error index outside boundaries: index: %d, max: %d\n", index, MAX_EVENT_COUNT);
        return -1; 
    }
    ics->current = index;
    return ics->current;
}

event_t *getEventAt(ics_t *ics, int index) 
{
    if (ics == NULL || ics->count == 0) return NULL; 

    size_t saved_index = ics->current;
    int current = setCurrentEvent(ics, index);
    if (current == -1) {
        return NULL;
    }

    event_t *event = getCurrentEvent(ics);
    ics->current = saved_index;

    return event;

}

// Comparison function for qsort
static int compare_events(const void *a, const void *b)
{
    const event_t *event_a = *(const event_t **)a;
    const event_t *event_b = *(const event_t **)b;

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
    qsort(ics->events, ics->count, sizeof(event_t*), compare_events);
}


bool atEnd(const ics_t *ics) {
    if (ics == NULL || ics->count == 0) {
        return true;
    }
    return ics->current >= (ics->capacity - 1) || ics->current >= (ics->count - 1);
}

void freeEventMembers(event_t * event) {
    if (event->summary != NULL) {
        free(event->summary);
        event->summary = NULL;
    }

    if (event->dtstart != NULL) {
        free(event->dtstart);
        event->dtstart = NULL;
    }

    if (event->dtend != NULL) {
        free(event->dtend);
        event->dtend = NULL;
    }

    if (event->rrule != NULL) {
        free(event->rrule);
        event->rrule = NULL;
    }

    if (event->interval != NULL) {
        free(event->interval);
        event->interval = NULL;
    }

    if (event->uid != NULL) {
        free(event->uid);
        event->uid = NULL;
    }
}

void freeEvent(event_t *event) {
    freeEventMembers(event);
    free(event);
}

event_t *cloneEvent(event_t *existing) {

    if(existing == NULL) return NULL;

    event_t *event = calloc(1, sizeof(event_t));

    event->summary = copyMember(existing->summary);
    event->dtstart = copyMember(existing->dtstart);
    event->dtend = copyMember(existing->dtend);
    event->rrule = copyMember(existing->rrule);
    event->interval = copyMember(existing->interval);
    event->uid = copyMember(existing->uid);
    
    event->tstart = existing->tstart;
    event->tend = existing->tend;

    return event;
}

char *copyMember(const char *source) {
    if (source == NULL) { 
        return NULL;
    }
    char *destination = calloc(strlen(source) + 1, sizeof(char)); // +1 for null terminator
    strcpy(destination, source);
    return destination;
}
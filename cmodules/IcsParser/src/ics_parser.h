#ifndef ICS_PARSE_H
#define ICS_PARSE_H

#include <stdbool.h>
#include <time.h>

// Define the structure to hold parsed event information
typedef struct {
    char *summary;
    char *dtstart;
    char *dtend;
    time_t tstart;
    time_t tend;
} event_t;

#define MAX_EVENT_COUNT 100
typedef struct {
    event_t *events;
    char * next;
    char * buffer;
    size_t count;
    size_t capacity;
    size_t current;
    time_t startTime;
    time_t endTime;
} ics_t;

size_t parseFile(ics_t * ics, const char *file_path);
size_t parse(ics_t * ics, const char *ics_data);

time_t setStartDate(ics_t *ics, const char *start);
time_t setEndDate(ics_t *ics, const char *end);

void initIcs(ics_t *ics);
void freeIcs(ics_t *ics);
void initIcsDates(ics_t *ics);
bool atEnd(ics_t *ics);

#endif // ICS_PARSE_H

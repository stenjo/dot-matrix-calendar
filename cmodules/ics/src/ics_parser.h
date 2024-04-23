#ifndef ICS_PARSE_H
#define ICS_PARSE_H

#include <stdbool.h>
#include <time.h>

// Define the structure to hold parsed event information
typedef struct {
    char *summary;
    char *dtstart;
    time_t tstart;
} event_t;

#define MAX_EVENT_COUNT 100
typedef struct {
    event_t events[MAX_EVENT_COUNT];
    const char * next;
    size_t count;
    size_t current;
    time_t startTime;
    time_t endTime;
} ics_t;

event_t getEvent(const char *ics_data, const char ** next);
size_t parse(ics_t * ics, const char *ics_data);
event_t getFirstEvent(ics_t *ics);
event_t getNextEvent(ics_t *ics);
event_t getLastEvent(ics_t *ics);
event_t getCurrentEvent(ics_t *ics);
size_t setCurrentEvent(ics_t *ics, size_t index);
event_t getEventAt(ics_t *ics, size_t index); 
event_t getNextEventInRange(ics_t *ics, const char *start_date_str, const char *end_date_str);
time_t setStartDate(ics_t *ics, const char *start);
time_t setEndDate(ics_t *ics, const char *end);

void initIcs(ics_t *ics);
void initIcsDates(ics_t *ics);
bool atEnd(ics_t *ics);

#endif // ICS_PARSE_H

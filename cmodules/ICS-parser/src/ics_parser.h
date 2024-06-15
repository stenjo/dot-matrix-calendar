#ifndef ICS_PARSER_H
#define ICS_PARSER_H

#include <stdbool.h>
#include <time.h>

// Define the structure to hold parsed event information
#define MAX_EVENT_COUNT 100

typedef struct {
    char *summary;
    char *dtstart;
    char *dtend;
    time_t tstart;
    time_t tend;
} event_t;

typedef struct {
    event_t events[MAX_EVENT_COUNT];
    char *buffer;
    size_t buffer_size;
    const char *next;
    size_t count;
    size_t current;
    time_t startTime;
    time_t endTime;
} ics_t;


/**
 * @brief: Parses a file and loads into the event list of the ics structure
 * 
 * @param ics calendar descriptor
 * @return number of events loaded
*/
size_t parseFile(ics_t * ics, const char *file_path);
size_t loadFileRef(const char *file_path);

void sortEventsByStart(ics_t *ics);
event_t getEvent(const char *ics_data, const char ** next);
size_t parse(ics_t *ics, const char *ics_data, size_t data_size);
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
void cleanIcs(ics_t *ics);
void initIcsDates(ics_t *ics);
bool atEnd(ics_t *ics);

#endif // ICS_PARSER_H

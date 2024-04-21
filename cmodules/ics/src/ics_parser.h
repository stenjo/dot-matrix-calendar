#ifndef ICS_PARSE_H
#define ICS_PARSE_H

// Define the structure to hold parsed event information
typedef struct {
    char *summary;
    char *dtstart;
} event_t;

#define MAX_EVENT_COUNT 100
typedef struct {
    event_t events[MAX_EVENT_COUNT];
    const char * next;
    size_t count;
    size_t current;
} ics_t;

event_t getEvent(const char *ics_data, const char ** next);
ics_t parse(const char *ics_data);
event_t getFirstEvent(ics_t *ics);
event_t getNextEvent(ics_t *ics);
event_t getLastEvent(ics_t *ics);
void initIcs(ics_t *ics);

#endif // ICS_PARSE_H

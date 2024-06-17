#ifndef ICS_EVENT_H
#define ICS_EVENT_H

#include "ics_parser.h"

char* extract_property(const char *data, const char *property);
event_t getEvent(const char *ics_data, const char ** next);
event_t getFirstEvent(ics_t *ics);
event_t getNextEvent(ics_t *ics);
event_t getLastEvent(ics_t *ics);
event_t getCurrentEvent(ics_t *ics);
size_t setCurrentEvent(ics_t *ics, size_t index);
event_t getEventAt(ics_t *ics, size_t index); 
event_t getNextEventInRange(ics_t *ics, const char *start_date_str, const char *end_date_str);
void sortEventsByStart(ics_t *ics);

#endif // ICS_EVENT_H

#ifndef ICS_EVENT_H
#define ICS_EVENT_H

#include <stdbool.h>
#include "event.h"
#include "ics.h"

void updateBuffer(const char *new_data);
void resetGetEvent(void);
event_t * getEvent(void);
char *extract_property(const char *data, const char *property, const char *end);
event_t *getFirstEvent(ics_t *ics);
event_t *getNextEvent(ics_t *ics);
event_t *getLastEvent(ics_t *ics);
event_t *getCurrentEvent(const ics_t *ics);
int setCurrentEvent(ics_t *ics, int index);
event_t *getEventAt(ics_t *ics, int index); 
event_t *getNextEventInRange(ics_t *ics, const char *start_date_str, const char *end_date_str);
void sortEventsByStart(ics_t *ics);
bool atEnd(const ics_t *ics);
void freeEventMembers(event_t *event);
void freeEvent(event_t *event);
event_t * cloneEvent(event_t * existing);
char *copyMember(const char *source);


#endif // ICS_EVENT_H

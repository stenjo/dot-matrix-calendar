#ifndef ICS_H
#define ICS_H

#include <time.h>
#include "event.h"

#define MAX_EVENT_COUNT 100
typedef struct {
    int count;
    int capacity;
    int current;
    time_t startTime;
    time_t endTime;
    event_t **events;
} ics_t;

#endif /*  ICS_H */
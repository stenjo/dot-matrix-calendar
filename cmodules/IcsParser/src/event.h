#ifndef EVENT_H
#define EVENT_H

#include <time.h>

// Define the structure to hold parsed event information
typedef struct {
    char *summary;
    char *dtstart;
    char *dtend;
    char *uid;
    time_t tstart;
    time_t tend;
} event_t;

#endif // EVENT_H
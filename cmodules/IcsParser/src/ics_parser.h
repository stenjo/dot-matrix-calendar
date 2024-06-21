#ifndef ICS_PARSE_H
#define ICS_PARSE_H

#include <time.h>
#include "ics.h"

size_t parseFile(ics_t * ics, const char *file_path);
size_t parse(ics_t * ics, const char *ics_data);

time_t setStartDate(ics_t *ics, const char *start);
time_t setEndDate(ics_t *ics, const char *end);

void initIcs(ics_t *ics);
void freeIcs(ics_t *ics);
void initIcsDates(ics_t *ics);


#endif // ICS_PARSE_H

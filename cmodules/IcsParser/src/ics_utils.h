#ifndef ICS_UTILS_H
#define ICS_UTILS_H

#include <stdbool.h>
#include <time.h>

bool parse_date_string(const char *date_string, struct tm *tm);

#endif // ICS_UTILS_H

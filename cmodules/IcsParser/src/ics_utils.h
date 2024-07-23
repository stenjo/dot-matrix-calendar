#ifndef ICS_UTILS_H
#define ICS_UTILS_H

#include <stdbool.h>
#include <time.h>

bool parse_date_string(const char *date_string, struct tm *tm);
time_t getTimeStamp(const char *date_str);
void updateDateStr(char * str, time_t time);
void nukeChar(char s[], char c);

#endif // ICS_UTILS_H

#ifndef ICS_UTILS_H
#define ICS_UTILS_H

#include <stdbool.h>
#include <time.h>

bool parse_date_string(const char *date_string, struct tm *tm);
int getYearFromTime(time_t timestamp);
time_t getTimeStamp(const char *date_str);
char * updateDateStr(char * str, time_t time);
time_t getMidnightTimeStamp(time_t time);
void nukeChar(char s[], char c);

#endif // ICS_UTILS_H

#ifndef ICS_UTILS_H
#define ICS_UTILS_H

#include <stdbool.h>
#include <time.h>

bool parse_date_string(const char *date_string, struct tm *tm);
time_t getTimeStamp(const char *date_str);
void updateDateStr(char * str, time_t time);
int replacechar(char *str, char orig, char rep);
void remove_all_chars(char* str, char c);
void nukechar(char s[], char c);

#endif // ICS_UTILS_H

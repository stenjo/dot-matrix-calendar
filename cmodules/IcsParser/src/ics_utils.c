#include "ics_utils.h"
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool parse_date_string(const char *date_str, struct tm *tm) {
    if (date_str == NULL) {
        return 0;
    }

    // Parse the date string.
    if (sscanf(date_str, "%4d%2d%2dT%2d%2d%2dZ", &tm->tm_year, &tm->tm_mon, &tm->tm_mday, &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6 
    || sscanf(date_str, "%4d%2d%2d", &tm->tm_year, &tm->tm_mon, &tm->tm_mday) == 3){
        tm->tm_year -= 1900; // tm_year is years since 1900
        tm->tm_mon -= 1;     // tm_mon is 0-based
        tm->tm_isdst = -1;   // Daylight saving information is unknown
        return true; // Successfully parsed
    }
    return false; // Failed to parse

}

time_t getTimeStamp(const char *date_str) {
    struct tm tm_event = {0};
    if (parse_date_string(date_str, &tm_event)) {
        time_t event_time = mktime(&tm_event);
        return event_time;
    }
    return -1; // Failed to parse
}

void updateDateStr(char * str, time_t time) {
        struct tm tm_event = {0};
        char buff[20];
    if (strlen(str) >= 15) {
        strftime(buff, 20, "%Y%m%dT%H%M%SZ", localtime_r(&time, &tm_event));
        strcpy(str, buff);
    } else if (strlen(str) >= 8) {
        strftime(buff, 10, "%Y%m%d", localtime_r(&time, &tm_event));
        strcpy(str, buff);
    }
}

void nukeChar(char s[], char c)
{
    size_t j = 0;
    for (size_t i = 0; s[i] != '\0'; ++i) {
        if (s[i] != c) {
            s[j] = s[i];
            ++j;
        }
    }
    s[j] = '\0';
}

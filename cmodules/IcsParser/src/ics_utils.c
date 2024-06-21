#include "ics_utils.h"
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

bool parse_date_string(const char *date_str, struct tm *tm) {
    if (date_str == NULL) {
        return 0;
    }

    // Parse the date string.
    if (sscanf(date_str, "%4d%2d%2dT%2d%2d%2dZ",
               &tm->tm_year,
               &tm->tm_mon,
               &tm->tm_mday,
               &tm->tm_hour,
               &tm->tm_min,
               &tm->tm_sec) == 6) {
        tm->tm_year -= 1900; // tm_year is years since 1900
        tm->tm_mon -= 1;     // tm_mon is 0-based
        tm->tm_isdst = -1;   // Daylight saving information is unknown
        return true; // Successfully parsed
    } else if (sscanf(date_str, "%4d%2d%2d",
               &tm->tm_year,
               &tm->tm_mon,
               &tm->tm_mday) == 3){

        tm->tm_year -= 1900; // tm_year is years since 1900
        tm->tm_mon -= 1;     // tm_mon is 0-based
        tm->tm_isdst = -1;   // Daylight saving information is unknown
        return true; // Successfully parsed
    }
    else
    {
        return false; // Failed to parse
    }
}



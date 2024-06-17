#include "ics_utils.h"
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

bool parse_date_string(const char *date_string, struct tm *tm) {
    if (strptime(date_string, "%Y%m%dT%H%M%S", tm) == NULL) {
        return false;
    }
    return true;
}


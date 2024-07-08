#ifdef TEST

#include <string.h>
#include <stdlib.h>
#include "unity.h"

#include "ics_parser.h"
#include "ics_event.h"
#include "ics_utils.h"

void setUp(void) {
    resetGetEvent();
}

void tearDown(void) {
    // Tear down code if needed
}


void test_parse_ics_from_file(void) {
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);

    const char startDate[] = "20240501T000000Z";
    const char endDate[] = "20240701T000000Z";

    time_t start = setStartDate(&ics, startDate);
    time_t end = setEndDate(&ics, endDate);

    size_t count = parseFile(&ics, "test/moon.ics");
    TEST_ASSERT_EQUAL(9, count);

    TEST_ASSERT_NOT_NULL(ics.events[0].summary);
    TEST_ASSERT_NOT_NULL(ics.events[0].dtstart);

    if (ics.events[0].summary) TEST_ASSERT_EQUAL_STRING("Last quarter 11:27am", ics.events[0].summary);
    if (ics.events[0].dtstart) TEST_ASSERT_EQUAL_STRING("20240501", ics.events[0].dtstart);

    freeIcs(&ics);
}



#endif // TEST

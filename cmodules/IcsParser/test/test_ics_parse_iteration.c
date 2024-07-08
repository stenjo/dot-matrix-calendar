#ifdef TEST

#include <string.h>
#include <stdlib.h>
#include "unity.h"

#include "ics_parser.h"
#include "ics_event.h"
#include "ics_utils.h"


const char *ics_data3 = "BEGIN:VCALENDAR\r\n"
                      "VERSION:2.0\r\n"
                      "BEGIN:VEVENT\r\n"
                      "SUMMARY:Meeting with John\r\n"
                      "DTSTART:20230412T160000Z\r\n"
                      "END:VEVENT\r\n"
                      "BEGIN:VEVENT\r\n"
                      "UID:http://2024.f2cal.com/#GP3_2024_qualifying\r\n"
                      "SUMMARY:F2: Kvalifisering (Emilia Romagna)\r\n"
                      "DTSTAMP:20240405T155803Z\r\n"
                      "DTSTART:20240517T140000Z\r\n"
                      "DTEND:20240517T143000Z\r\n"
                      "SEQUENCE:2024\r\n"
                      "GEO:44.344576;11.713808\r\n"
                      "LOCATION:Imola\r\n"
                      "STATUS:CONFIRMED\r\n"
                      "CATEGORIES:Kvalifisering,F2\r\n"
                      "END:VEVENT\r\n"
                      "BEGIN:VEVENT\r\n"
                      "UID:http://2024.f2cal.com/#GP4_2024_feature\r\n"
                      "SUMMARY:F2: Feature (Monaco)\r\n"
                      "DTSTAMP:20240405T155803Z\r\n"
                      "DTSTART:20240526T084000Z\r\n"
                      "DTEND:20240526T094000Z\r\n"
                      "SEQUENCE:2024\r\n"
                      "GEO:43.7338;7.4215\r\n"
                      "LOCATION:Monte Carlo\r\n"
                      "STATUS:CONFIRMED\r\n"
                      "CATEGORIES:Feature,F2\r\n"
                      "END:VEVENT\r\n"
                      "END:VCALENDAR";

void setUp(void) {
    resetGetEvent();
}

void tearDown(void) {
    // Tear down code if needed
}


void test_parse_ShouldReturnEventListIterationThreeEvents(void) {
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);
    size_t count = parseIcs(&ics, ics_data3);
    event_t event = getFirstEvent(&ics);

    TEST_ASSERT_EQUAL(3, count);
    TEST_ASSERT_EQUAL(3, ics.count);

    TEST_ASSERT_EQUAL_STRING("Meeting with John", event.summary);
    TEST_ASSERT_EQUAL_STRING("20230412T160000Z", event.dtstart);

    event = getNextEvent(&ics);
    TEST_ASSERT_EQUAL_STRING("F2: Kvalifisering (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T140000Z", event.dtstart);

    event = getNextEvent(&ics);
    TEST_ASSERT_EQUAL_STRING("F2: Feature (Monaco)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240526T084000Z", event.dtstart);

    event = getFirstEvent(&ics);
    TEST_ASSERT_EQUAL_STRING("Meeting with John", event.summary);
    TEST_ASSERT_EQUAL_STRING("20230412T160000Z", event.dtstart);

    freeIcs(&ics);
}



#endif // TEST

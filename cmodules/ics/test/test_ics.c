/* Testing file */
#include <string.h>
#include <stdlib.h>
#include "../src/ics_parser.h"
#include "../src/ics_file.h"
#include "unity.h"

const char *ics_data = "BEGIN:VCALENDAR\r\n"
                      "VERSION:2.0\r\n"
                      "BEGIN:VEVENT\r\n"
                      "SUMMARY:Meeting with John\r\n"
                      "DTSTART:20230412T160000Z\r\n"
                      "END:VEVENT\r\n"
                      "END:VCALENDAR";

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


void setUp(void)
{
}

void tearDown(void)
{
}


void test_ics_parse_ShouldReturnEventSummaryAndStart(void)
{
    const char * next = NULL;

    event_t event = getEvent(ics_data, &next);
    TEST_ASSERT_EQUAL_STRING("Meeting with John",event.summary);
    TEST_ASSERT_EQUAL_STRING("20230412T160000Z", event.dtstart);
    TEST_ASSERT_EQUAL(17,strlen(event.summary));
}

void test_parse_ShouldReturnEventList(void) {
    ics_t ics = parse(ics_data);
    TEST_ASSERT_EQUAL_STRING("Meeting with John",ics.events[0].summary);
    TEST_ASSERT_EQUAL_STRING("20230412T160000Z", ics.events[0].dtstart);
    TEST_ASSERT_EQUAL(1,ics.count);
        
}

void test_parse_ShouldReturnEventListIterationOneEvent(void) {
    ics_t ics = parse(ics_data);
    event_t event = getFirstEvent(&ics);

    TEST_ASSERT_EQUAL_STRING("Meeting with John",event.summary);
    TEST_ASSERT_EQUAL_STRING("20230412T160000Z", event.dtstart);
        
}
void test_parse_ShouldReturnEventListIterationThreeEvents(void) {
    ics_t ics = parse(ics_data3);
    event_t event = getFirstEvent(&ics);

    TEST_ASSERT_EQUAL(3,ics.count);

    TEST_ASSERT_EQUAL_STRING("Meeting with John",event.summary);
    TEST_ASSERT_EQUAL_STRING("20230412T160000Z", event.dtstart);

    event = getNextEvent(&ics);
    TEST_ASSERT_EQUAL_STRING("F2: Kvalifisering (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T140000Z", event.dtstart);

    event = getNextEvent(&ics);
    TEST_ASSERT_EQUAL_STRING("F2: Feature (Monaco)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240526T084000Z", event.dtstart);
}

void test_parse_ics_from_file(void)
{
    ics_t ics =  parseFile("../test/test_event.ics");

    TEST_ASSERT_NOT_NULL(ics.events[0].summary);
    TEST_ASSERT_NOT_NULL(ics.events[0].dtstart);

    if (ics.events[0].summary) TEST_ASSERT_EQUAL_STRING("Test Event", ics.events[0].summary);
    if (ics.events[0].dtstart) TEST_ASSERT_EQUAL_STRING("20230412T160000Z", ics.events[0].dtstart);

    // Free any allocated resources by the parse function
    if (ics.events[0].summary) free(ics.events[0].summary);
    if (ics.events[0].dtstart) free(ics.events[0].dtstart);
}

void test_parse_ics_from_f2_file(void)
{
    ics_t ics = parseFile("../test/f2-calendar_p_q_sprint_feature.ics");

    TEST_ASSERT_NOT_NULL(ics.events[0].summary);
    TEST_ASSERT_NOT_NULL(ics.events[0].dtstart);

    TEST_ASSERT_EQUAL_STRING("F2: Practice (Bahrain)", ics.events[0].summary);
    TEST_ASSERT_EQUAL_STRING("20240229T090500Z", ics.events[0].dtstart);
    TEST_ASSERT_EQUAL(56,ics.count);

    // Free any allocated resources by the parse function
    if (ics.events[0].summary) free(ics.events[0].summary);
    if (ics.events[0].dtstart) free(ics.events[0].dtstart);
}

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

    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);
    parse(&ics, ics_data);
    TEST_ASSERT_EQUAL_STRING("Meeting with John",ics.events[0].summary);
    TEST_ASSERT_EQUAL_STRING("20230412T160000Z", ics.events[0].dtstart);
    TEST_ASSERT_EQUAL(1,ics.count);
    freeIcs(&ics);
        
}

void test_parse_ShouldReturnEventListIterationOneEvent(void) {
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);
    size_t count = parse(&ics, ics_data);
    event_t event = getFirstEvent(&ics);
    TEST_ASSERT_EQUAL(1,count);

    TEST_ASSERT_EQUAL_STRING("Meeting with John",event.summary);
    TEST_ASSERT_EQUAL_STRING("20230412T160000Z", event.dtstart);
        
    freeIcs(&ics);
}
void test_parse_ShouldReturnEventListIterationThreeEvents(void) {
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);
    size_t count = parse(&ics, ics_data3);
    event_t event = getFirstEvent(&ics);

    TEST_ASSERT_EQUAL(3,count);
    TEST_ASSERT_EQUAL(3,ics.count);

    TEST_ASSERT_EQUAL_STRING("Meeting with John",event.summary);
    TEST_ASSERT_EQUAL_STRING("20230412T160000Z", event.dtstart);

    event = getNextEvent(&ics);
    TEST_ASSERT_EQUAL_STRING("F2: Kvalifisering (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T140000Z", event.dtstart);

    event = getNextEvent(&ics);
    TEST_ASSERT_EQUAL_STRING("F2: Feature (Monaco)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240526T084000Z", event.dtstart);

    event = getFirstEvent(&ics);
    TEST_ASSERT_EQUAL_STRING("Meeting with John",event.summary);
    TEST_ASSERT_EQUAL_STRING("20230412T160000Z", event.dtstart);

    freeIcs(&ics);
}
void test_setCurrentEvent_verifyIndexWithinBounds(void) {
    
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);
    size_t count = parse(&ics, ics_data3);

    TEST_ASSERT_EQUAL(3,count);

    size_t index = setCurrentEvent(&ics, 2);
    TEST_ASSERT_EQUAL(2,index);
    
    event_t event = getCurrentEvent(&ics);
    TEST_ASSERT_EQUAL_STRING("F2: Feature (Monaco)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240526T084000Z", event.dtstart);

    index = setCurrentEvent(&ics, 3);
    TEST_ASSERT_EQUAL(-1,index);
    
    index = setCurrentEvent(&ics, -1);
    TEST_ASSERT_EQUAL(-1,index);

    freeIcs(&ics);
    
}

void test_getEventAt_verifyGettingEventAtGivenIndex(void) {
    
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);
    size_t count = parse(&ics, ics_data3);

    TEST_ASSERT_EQUAL(3,count);

    event_t event = getEventAt(&ics, 1);
    TEST_ASSERT_EQUAL_STRING("F2: Kvalifisering (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T140000Z", event.dtstart);
    
}

void test_parse_ics_from_file(void)
{
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);
    size_t count = parseFile(&ics, "../test/test_event.ics");
    TEST_ASSERT_EQUAL(1,count);

    TEST_ASSERT_NOT_NULL(ics.events[0].summary);
    TEST_ASSERT_NOT_NULL(ics.events[0].dtstart);

    if (ics.events[0].summary) TEST_ASSERT_EQUAL_STRING("Test Event", ics.events[0].summary);
    if (ics.events[0].dtstart) TEST_ASSERT_EQUAL_STRING("20230412T160000Z", ics.events[0].dtstart);

    freeIcs(&ics);
}

void test_parse_ics_from_f2_file(void)
{
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);
    size_t count = parseFile(&ics, "../test/f2-calendar_p_q_sprint_feature.ics");
    TEST_ASSERT_EQUAL(56,count);

    TEST_ASSERT_NOT_NULL(ics.events[0].summary);
    TEST_ASSERT_NOT_NULL(ics.events[0].dtstart);

    TEST_ASSERT_EQUAL_STRING("F2: Practice (Bahrain)", ics.events[0].summary);
    TEST_ASSERT_EQUAL_STRING("20240229T090500Z", ics.events[0].dtstart);
    TEST_ASSERT_EQUAL(56,ics.count);

    freeIcs(&ics);
}
void test_getNExtEventInRange(void)
{
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);

    const char startDate[] = "20240501T000000Z";
    const char endDate[] =  "20240701T000000Z";

    time_t start = setStartDate(&ics, startDate);
    TEST_ASSERT_EQUAL(1714514400,start);


    size_t count = parseFile(&ics, "../test/f2-calendar_p_q_sprint_feature.ics");
    TEST_ASSERT_EQUAL(44,count);

    event_t event = getFirstEvent(&ics);

    TEST_ASSERT_EQUAL_STRING("F2: Practice (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T090500Z", event.dtstart);
    TEST_ASSERT_EQUAL(44,ics.count);

    initIcs(&ics);
    time_t end = setEndDate(&ics, endDate);
    TEST_ASSERT_EQUAL(1719784800,end);

    count = parseFile(&ics, "../test/f2-calendar_p_q_sprint_feature.ics");
    TEST_ASSERT_EQUAL(16,ics.count);

    event = getFirstEvent(&ics);

    TEST_ASSERT_EQUAL_STRING("F2: Practice (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T090500Z", event.dtstart);

    event = getLastEvent(&ics);
    

    TEST_ASSERT_EQUAL_STRING("F2: Feature (Austrian)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240630T094000Z", event.dtstart);

}

void test_sortEvents_sortAllFilteredEventsByStartTime(void)
{
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);

    const char startDate[] = "20240501T000000Z";
    const char endDate[] =  "20240701T000000Z";

    time_t start = setStartDate(&ics, startDate);
    TEST_ASSERT_EQUAL(1714514400,start);
    time_t end = setEndDate(&ics, endDate);
    TEST_ASSERT_EQUAL(1719784800,end);

    int count = parseFile(&ics, "../test/f2-calendar_p_q_sprint_feature.ics");
    event_t event = getFirstEvent(&ics);
    TEST_ASSERT_EQUAL(16,count);

    TEST_ASSERT_EQUAL_STRING("F2: Practice (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T090500Z", event.dtstart);
    TEST_ASSERT_EQUAL(16,ics.count);

    sortEventsByStart(&ics);

    event = getFirstEvent(&ics);
    TEST_ASSERT_EQUAL(16,ics.count);

    TEST_ASSERT_EQUAL_STRING("F2: Practice (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T090500Z", event.dtstart);
    TEST_ASSERT_EQUAL(16,ics.count);
}

void test_endDate_verifyEndDateAvailableOnEvents(void)
{
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);

    const char startDate[] = "20240501T000000Z";
    const char endDate[] =  "20240701T000000Z";

    time_t start = setStartDate(&ics, startDate);
    TEST_ASSERT_EQUAL(1714514400,start);
    time_t end = setEndDate(&ics, endDate);
    TEST_ASSERT_EQUAL(1719784800,end);

    int count = parseFile(&ics, "../test/f2-calendar_p_q_sprint_feature.ics");
    event_t event = getFirstEvent(&ics);
    TEST_ASSERT_EQUAL(16,count);

    TEST_ASSERT_EQUAL_STRING("F2: Practice (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T095000Z", event.dtend);
}

void test_withLargeCalendar(void)
{
        ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);

    const char startDate[] = "20240401T000000Z";
    const char endDate[] =  "20240701T000000Z";

    time_t start = setStartDate(&ics, startDate);
    TEST_ASSERT_EQUAL(1711922400,start);
    time_t end = setEndDate(&ics, endDate);
    TEST_ASSERT_EQUAL(1719784800,end);

    int count = parseFile(&ics, "../test/basic.ics");
    event_t event = getFirstEvent(&ics);
    TEST_ASSERT_EQUAL(24,count);

    TEST_ASSERT_EQUAL_STRING("Pokebowl", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240418T053000Z", event.dtstart);

}

void test_withMultipleCalendars(void)
{
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);

    const char startDate[] = "20240401T000000Z";
    const char endDate[] =  "20241201T000000Z";

    time_t start = setStartDate(&ics, startDate);
    TEST_ASSERT_EQUAL(1711926000,start);
    time_t end = setEndDate(&ics, endDate);
    TEST_ASSERT_EQUAL(1733004000,end);

    int count = parseFile(&ics, "../test/f2-calendar_p_q_sprint_feature.ics");
    TEST_ASSERT_EQUAL(39,count);

    event_t event = getFirstEvent(&ics);
    TEST_ASSERT_EQUAL_STRING("F2: Practice (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T090500Z", event.dtstart);

    count = parseFile(&ics, "../test/f3-calendar_p_q_sprint_feature.ics");
    TEST_ASSERT_EQUAL(70,count);

    sortEventsByStart(&ics);

    event = getFirstEvent(&ics);
    TEST_ASSERT_EQUAL_STRING("F3: Practice (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T075500Z", event.dtstart);

    event = getNextEvent(&ics);
    TEST_ASSERT_EQUAL_STRING("F2: Practice (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T090500Z", event.dtstart);

    count = parseFile(&ics, "../test/basic.ics");
    sortEventsByStart(&ics);

    event = getFirstEvent(&ics);
    TEST_ASSERT_EQUAL_STRING("Vegetar taco", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240418T053000Z", event.dtstart);

}
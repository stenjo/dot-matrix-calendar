#ifdef TEST

#include <string.h>
#include <stdlib.h>
#include "unity.h"

#include "ics_parser.h"
#include "ics_event.h"
#include "ics_utils.h"

const char *ics_data = "BEGIN:VCALENDAR\r\n"
                      "VERSION:2.0\r\n"
                      "BEGIN:VEVENT\r\n"
                      "SUMMARY:Meeting with John\r\n"
                      "DTSTART:20230412T160000Z\r\n"
                      "END:VEVENT\r\n"
                      "END:VCALENDAR";

const char *ics_all_day = "BEGIN:VCALENDAR\r\n"
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

void setUp(void) {
    resetGetEvent();
}

void tearDown(void) {
    // Tear down code if needed
}

void test_ics_parse_ShouldReturnEventSummaryAndStart(void) {
    updateBuffer(ics_data);
    event_t event = getEvent();
    TEST_ASSERT_EQUAL_STRING("Meeting with John", event.summary);
    TEST_ASSERT_EQUAL_STRING("20230412T160000Z", event.dtstart);
    TEST_ASSERT_EQUAL(17, strlen(event.summary));
}

void test_parse_ShouldReturnEventList(void) {
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);
    parse(&ics, ics_data);
    TEST_ASSERT_EQUAL_STRING("Meeting with John", ics.events[0].summary);
    TEST_ASSERT_EQUAL_STRING("20230412T160000Z", ics.events[0].dtstart);
    TEST_ASSERT_EQUAL(1, ics.count);
    freeIcs(&ics);
}

void test_parse_ShouldReturnEventListIterationOneEvent(void) {
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);
    size_t count = parse(&ics, ics_data);
    event_t event = getFirstEvent(&ics);
    TEST_ASSERT_EQUAL(1, count);

    TEST_ASSERT_EQUAL_STRING("Meeting with John", event.summary);
    TEST_ASSERT_EQUAL_STRING("20230412T160000Z", event.dtstart);

    freeIcs(&ics);
}

void test_parse_ShouldReturnEventListIterationThreeEvents(void) {
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);
    size_t count = parse(&ics, ics_data3);
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

void test_setCurrentEvent_verifyIndexWithinBounds(void) {
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);
    size_t count = parse(&ics, ics_data3);

    TEST_ASSERT_EQUAL(3, count);

    size_t index = setCurrentEvent(&ics, 2);
    TEST_ASSERT_EQUAL(2, index);

    event_t event = getCurrentEvent(&ics);
    TEST_ASSERT_EQUAL_STRING("F2: Feature (Monaco)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240526T084000Z", event.dtstart);

    index = setCurrentEvent(&ics, 3);
    TEST_ASSERT_EQUAL(-1, index);

    index = setCurrentEvent(&ics, -1);
    TEST_ASSERT_EQUAL(-1, index);

    freeIcs(&ics);
}

void test_getEventAt_verifyGettingEventAtGivenIndex(void) {
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);
    size_t count = parse(&ics, ics_data3);

    TEST_ASSERT_EQUAL(3, count);

    event_t event = getEventAt(&ics, 1);
    TEST_ASSERT_EQUAL_STRING("F2: Kvalifisering (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T140000Z", event.dtstart);
    freeIcs(&ics);
}

void test_parse_ics_from_file(void) {
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);
    size_t count = parseFile(&ics, "test/test_event.ics");
    TEST_ASSERT_EQUAL(1, count);

    TEST_ASSERT_NOT_NULL(ics.events[0].summary);
    TEST_ASSERT_NOT_NULL(ics.events[0].dtstart);

    if (ics.events[0].summary) TEST_ASSERT_EQUAL_STRING("Test Event", ics.events[0].summary);
    if (ics.events[0].dtstart) TEST_ASSERT_EQUAL_STRING("20230412T160000Z", ics.events[0].dtstart);

    freeIcs(&ics);
}

void test_parse_ics_from_f2_file(void) {
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);
    size_t count = parseFile(&ics, "test/f2-calendar_p_q_sprint_feature.ics");
    TEST_ASSERT_EQUAL(56, count);

    TEST_ASSERT_NOT_NULL(ics.events[0].summary);
    TEST_ASSERT_NOT_NULL(ics.events[0].dtstart);

    TEST_ASSERT_EQUAL_STRING("F2: Practice (Bahrain)", ics.events[0].summary);
    TEST_ASSERT_EQUAL_STRING("20240229T090500Z", ics.events[0].dtstart);
    TEST_ASSERT_EQUAL(56, ics.count);

    freeIcs(&ics);
}

void test_getNextEventInRange(void) {
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);

    const char startDate[] = "20240501T000000Z";
    const char endDate[] = "20240701T000000Z";

    time_t start = setStartDate(&ics, startDate);
    TEST_ASSERT_EQUAL(1714514400, start);

    size_t count = parseFile(&ics, "test/f2-calendar_p_q_sprint_feature.ics");
    TEST_ASSERT_EQUAL(44, count);

    event_t event = getFirstEvent(&ics);

    TEST_ASSERT_EQUAL_STRING("F2: Practice (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T090500Z", event.dtstart);
    TEST_ASSERT_EQUAL(44, ics.count);
    resetGetEvent();
    initIcs(&ics);
    setStartDate(&ics, startDate);
    time_t end = setEndDate(&ics, endDate);
    TEST_ASSERT_EQUAL(1719784800, end);
    count = parseFile(&ics, "test/f2-calendar_p_q_sprint_feature.ics");
    TEST_ASSERT_EQUAL(16, ics.count);

    event = getFirstEvent(&ics);

    TEST_ASSERT_EQUAL_STRING("F2: Practice (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T090500Z", event.dtstart);

    event = getLastEvent(&ics);

    TEST_ASSERT_EQUAL_STRING("F2: Feature (Austrian)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240630T094000Z", event.dtstart);
    freeIcs(&ics);
}

void test_sortEvents_sortAllFilteredEventsByStartTime(void) {
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);

    const char startDate[] = "20240501T000000Z";
    const char endDate[] = "20240701T000000Z";

    time_t start = setStartDate(&ics, startDate);
    TEST_ASSERT_EQUAL(1714514400, start);
    time_t end = setEndDate(&ics, endDate);
    TEST_ASSERT_EQUAL(1719784800, end);

    int count = parseFile(&ics, "test/f2-calendar_p_q_sprint_feature.ics");
    event_t event = getFirstEvent(&ics);
    TEST_ASSERT_EQUAL(16, count);

    TEST_ASSERT_EQUAL_STRING("F2: Practice (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T090500Z", event.dtstart);
    TEST_ASSERT_EQUAL(16, ics.count);

    sortEventsByStart(&ics);

    event = getFirstEvent(&ics);
    TEST_ASSERT_EQUAL(16, ics.count);

    TEST_ASSERT_EQUAL_STRING("F2: Practice (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T090500Z", event.dtstart);
    TEST_ASSERT_EQUAL(16, ics.count);
    freeIcs(&ics);
}

void test_endDate_verifyEndDateAvailableOnEvents(void) {
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);

    const char startDate[] = "20240501T000000Z";
    const char endDate[] = "20240701T000000Z";

    time_t start = setStartDate(&ics, startDate);
    TEST_ASSERT_EQUAL(1714514400, start);
    time_t end = setEndDate(&ics, endDate);
    TEST_ASSERT_EQUAL(1719784800, end);

    int count = parseFile(&ics, "test/f2-calendar_p_q_sprint_feature.ics");
    event_t event = getFirstEvent(&ics);
    TEST_ASSERT_EQUAL(16, count);

    TEST_ASSERT_EQUAL_STRING("F2: Practice (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T095000Z", event.dtend);
    freeIcs(&ics);
}

void test_withLargeCalendar(void) {
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);

    const char startDate[] = "20240401T000000Z";
    const char endDate[] = "20240701T000000Z";

    time_t start = setStartDate(&ics, startDate);
    TEST_ASSERT_EQUAL(1711922400, start);
    time_t end = setEndDate(&ics, endDate);
    TEST_ASSERT_EQUAL(1719784800, end);

    int count = parseFile(&ics, "test/basic.ics");
    event_t event = getFirstEvent(&ics);
    TEST_ASSERT_EQUAL(10, count);

    TEST_ASSERT_EQUAL_STRING("Service på bilen", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240418T053000Z", event.dtstart);
    freeIcs(&ics);
}

void test_withMultipleCalendars(void) {
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);

    const char startDate[] = "20240401T000000Z";
    const char endDate[] = "20241201T000000Z";

    time_t start = setStartDate(&ics, startDate);
    TEST_ASSERT_EQUAL(1711922400, start);
    time_t end = setEndDate(&ics, endDate);
    TEST_ASSERT_EQUAL(1733007600, end);

    int count = parseFile(&ics, "test/f2-calendar_p_q_sprint_feature.ics");
    TEST_ASSERT_EQUAL(39, count);

    event_t event = getFirstEvent(&ics);
    TEST_ASSERT_EQUAL_STRING("F2: Practice (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T090500Z", event.dtstart);

    count = parseFile(&ics, "test/f3-calendar_p_q_sprint_feature.ics");
    TEST_ASSERT_EQUAL(70, count);

    sortEventsByStart(&ics);

    event = getFirstEvent(&ics);
    TEST_ASSERT_EQUAL_STRING("F3: Practice (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T075500Z", event.dtstart);

    event = getNextEvent(&ics);
    TEST_ASSERT_EQUAL_STRING("F2: Practice (Emilia Romagna)", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240517T090500Z", event.dtstart);

    count = parseFile(&ics, "test/basic.ics");
    sortEventsByStart(&ics);

    event = getFirstEvent(&ics);
    TEST_ASSERT_EQUAL_STRING("Rød curry med torsk", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240416", event.dtstart);
    freeIcs(&ics);
}

void test_parse_allDayEvents(void) {
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);

    int count = parseFile(&ics, "test/allday.ics");
    TEST_ASSERT_EQUAL(2, count);

    event_t event = getFirstEvent(&ics);
    TEST_ASSERT_EQUAL_STRING("Første eventet", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240425T070000Z", event.dtstart);
    event = getNextEvent(&ics);
    // TEST_ASSERT_EQUAL_STRING("I Åsane har vi både færøymål, låglønnsnæring og skjærgårdsøl!", event.summary);
    TEST_ASSERT_EQUAL_STRING("20240427", event.dtstart);
    TEST_ASSERT_EQUAL_STRING("20240428", event.dtend);
    freeIcs(&ics);
}

void test_parse_with_chunks(void) {
    // Create a dummy ics_t structure
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);

    // Create chunks of ics_data
    const char *chunk1 = "BEGIN:VEVENT\r\nSUMMARY:Event 1\r\nDTSTART:";
    const char *chunk2 = "20230615T090000\r\nDTEND:20230615T100000\r\nEND:VEVENT\r\nBEGIN:VEVENT\r\n";
    const char *chunk3 = "SUMMARY:Event 2\r\nDTSTART:20230616T090000\r\nDTEND:20230616T100000\r\nEND:VEVENT\r\n";

    // Call the parse function with each chunk of data
    parse(&ics, chunk1);
    parse(&ics, chunk2);
    parse(&ics, chunk3);

    // Verify that the events were parsed correctly
    TEST_ASSERT_EQUAL(2, ics.count);

    TEST_ASSERT_NOT_NULL(ics.events[0].summary);
    TEST_ASSERT_EQUAL_STRING("Event 1", ics.events[0].summary);
    TEST_ASSERT_NOT_NULL(ics.events[0].dtstart);
    TEST_ASSERT_EQUAL_STRING("20230615T090000", ics.events[0].dtstart);
    TEST_ASSERT_NOT_NULL(ics.events[0].dtend);
    TEST_ASSERT_EQUAL_STRING("20230615T100000", ics.events[0].dtend);

    TEST_ASSERT_NOT_NULL(ics.events[1].summary);
    TEST_ASSERT_EQUAL_STRING("Event 2", ics.events[1].summary);
    TEST_ASSERT_NOT_NULL(ics.events[1].dtstart);
    TEST_ASSERT_EQUAL_STRING("20230616T090000", ics.events[1].dtstart);
    TEST_ASSERT_NOT_NULL(ics.events[1].dtend);
    TEST_ASSERT_EQUAL_STRING("20230616T100000", ics.events[1].dtend);

    // Clean up
    freeIcs(&ics);
}


#endif // TEST

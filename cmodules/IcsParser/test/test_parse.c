#ifdef TEST

#include "unity.h"
#include "ics_parser.h"
#include "mock_ics_event.h"
#include <stdlib.h>
#include <string.h>
#include "ics_utils.h"

void setUp(void) {
    // Set up code if needed
}

void tearDown(void) {
    // Tear down code if needed
}

void test_parse_should_handle_single_event(void) {
    ics_t ics;
    initIcs(&ics);

    const char *data = "BEGIN:VEVENT\r\nSUMMARY:Meeting with John\r\nDTSTART:20230615T090000\r\nDTEND:20230615T100000\r\nEND:VEVENT\r\n";

    event_t mock_event;
    mock_event.summary = strdup("Meeting with John");
    mock_event.dtstart = strdup("20230615T090000");
    mock_event.dtend = strdup("20230615T100000");

    // Initialize buffer with data and then call getEvent
    updateBuffer_Expect(data);
    getEvent_ExpectAndReturn(&mock_event);
    getEvent_ExpectAndReturn(NULL);
    freeEvent_Ignore();
    resetGetEvent_Ignore();

    size_t count = parseIcs(&ics, data);

    TEST_ASSERT_EQUAL(1, count);
    TEST_ASSERT_EQUAL_STRING("Meeting with John", ics.events[0]->summary);
    TEST_ASSERT_EQUAL_STRING("20230615T090000", ics.events[0]->dtstart);
    TEST_ASSERT_EQUAL_STRING("20230615T100000", ics.events[0]->dtend);

    freeIcs(&ics);
}
void test_parse_should_handle_all_day_ongoing_event_today(void) {

    // Arrange
    ics_t ics;
    initIcs(&ics);

    const char *data =  "BEGIN:VEVENT\r\n"
                        "DTSTART;VALUE=DATE:20240427\r\n"
                        "DTEND;VALUE=DATE:20240428\r\n"
                        "DTSTAMP:20240425T055359Z\r\n"
                        "SUMMARY:SUMMARY:Meeting with John\r\n"
                        "END:VEVENT\r\n";

    event_t mock_event;
    mock_event.summary = strdup("Meeting with John");
    mock_event.dtstart = strdup("20240427");
    mock_event.dtend = strdup("20240428");
    mock_event.rrule = NULL;
    mock_event.interval = NULL;
    mock_event.tstart = getTimeStamp(mock_event.dtstart);
    mock_event.tend = getTimeStamp(mock_event.dtend);

    setStartDate(&ics, "20240427T090000");
    setEndDate(&ics, "20240429T090000");

    // Initialize buffer with data and then call getEvent
    updateBuffer_Expect(data);
    getEvent_ExpectAndReturn(&mock_event);
    getEvent_ExpectAndReturn(NULL);
    freeEvent_Ignore();
    resetGetEvent_Ignore();

    // Act
    size_t count = parseIcs(&ics, data);

    // Assert
    TEST_ASSERT_EQUAL(1, count);
    TEST_ASSERT_EQUAL_STRING("Meeting with John", ics.events[0]->summary);
    TEST_ASSERT_EQUAL_STRING("20240427", ics.events[0]->dtstart);
    TEST_ASSERT_EQUAL_STRING("20240428", ics.events[0]->dtend);

    freeIcs(&ics);
}

void test_parse_should_handle_multiple_events(void) {
    ics_t ics;
    initIcs(&ics);

    const char *data = "BEGIN:VEVENT\r\nSUMMARY:Event 1\r\nDTSTART:20230615T090000\r\nDTEND:20230615T100000\r\nEND:VEVENT\r\nBEGIN:VEVENT\r\nSUMMARY:Event 2\r\nDTSTART:20230616T090000\r\nDTEND:20230616T100000\r\nEND:VEVENT\r\n";

    event_t mock_event1;
    mock_event1.summary = strdup("Event 1");
    mock_event1.dtstart = strdup("20230615T090000");
    mock_event1.dtend = strdup("20230615T100000");

    event_t mock_event2;
    mock_event2.summary = strdup("Event 2");
    mock_event2.dtstart = strdup("20230616T090000");
    mock_event2.dtend = strdup("20230616T100000");

    // Initialize buffer with data and then call getEvent
    updateBuffer_Expect(data);
    getEvent_ExpectAndReturn(&mock_event1);
    getEvent_ExpectAndReturn(&mock_event2);
    getEvent_ExpectAndReturn(NULL);
    freeEventMembers_Ignore();
    freeEvent_Ignore();
    resetGetEvent_Ignore();

    size_t count = parseIcs(&ics, data);

    TEST_ASSERT_EQUAL(2, count);
    TEST_ASSERT_EQUAL_STRING("Event 1", ics.events[0]->summary);
    TEST_ASSERT_EQUAL_STRING("20230615T090000", ics.events[0]->dtstart);
    TEST_ASSERT_EQUAL_STRING("20230615T100000", ics.events[0]->dtend);
    TEST_ASSERT_EQUAL_STRING("Event 2", ics.events[1]->summary);
    TEST_ASSERT_EQUAL_STRING("20230616T090000", ics.events[1]->dtstart);
    TEST_ASSERT_EQUAL_STRING("20230616T100000", ics.events[1]->dtend);

    freeIcs(&ics);
}

void test_parse_should_handle_no_events(void) {
    ics_t ics;
    initIcs(&ics);

    const char *data = "SUMMARY:Meeting with John\r\nDTSTART:20230615T090000\r\n";

    // Initialize buffer with data and then call getEvent
    updateBuffer_Expect(data);
    getEvent_ExpectAndReturn(NULL);
    resetGetEvent_Ignore();

    size_t count = parseIcs(&ics, data);

    TEST_ASSERT_EQUAL(0, count);
    freeIcs(&ics);
}

void test_parse_should_handle_incomplete_event(void) {
    ics_t ics;
    initIcs(&ics);

    const char *data = "BEGIN:VEVENT\r\nSUMMARY:Meeting with John\r\nDTSTART:20230615T090000\r\n";

    // Initialize buffer with data and then call getEvent
    updateBuffer_Expect(data);
    getEvent_ExpectAndReturn(NULL);
    resetGetEvent_Ignore();

    size_t count = parseIcs(&ics, data);

    TEST_ASSERT_EQUAL(0, count);
    freeIcs(&ics);
}

void test_parse_should_handle_multiple_calls(void) {
    ics_t ics;
    initIcs(&ics);

    const char *data1 = "BEGIN:VEVENT\r\nSUMMARY:Event 1\r\nDTSTART:20230615T090000\r\n";
    const char *data2 = "DTEND:20230615T100000\r\nEND:VEVENT\r\nBEGIN:VEVENT\r\nSUMMARY:Event 2\r\nDTSTART:20230616T090000\r\nDTEND:20230616T100000\r\nEND:VEVENT\r\n";

    event_t mock_event1;
    mock_event1.summary = strdup("Event 1");
    mock_event1.dtstart = strdup("20230615T090000");
    mock_event1.dtend = strdup("20230615T100000");
    mock_event1.tstart = getTimeStamp(mock_event1.dtstart);
    mock_event1.tend = getTimeStamp(mock_event1.dtend);

    event_t mock_event2;
    mock_event2.summary = strdup("Event 2");
    mock_event2.dtstart = strdup("20230616T090000");
    mock_event2.dtend = strdup("20230616T100000");
    mock_event2.tstart = getTimeStamp(mock_event2.dtstart);
    mock_event2.tend = getTimeStamp(mock_event2.dtend);

    // Initialize buffer with data1 and call getEvent
    updateBuffer_Expect(data1);
    getEvent_ExpectAndReturn(&mock_event1);
    getEvent_ExpectAndReturn(NULL);

    // Initialize buffer with data2 and call getEvent
    updateBuffer_Expect(data2);
    getEvent_ExpectAndReturn(&mock_event2);
    getEvent_ExpectAndReturn(NULL);
    freeEvent_Ignore();
    freeEventMembers_Ignore();
    resetGetEvent_Ignore();

    parseIcs(&ics, data1);
    size_t count = parseIcs(&ics, data2);

    TEST_ASSERT_EQUAL(2, count);
    TEST_ASSERT_EQUAL_STRING("Event 1", ics.events[0]->summary);
    TEST_ASSERT_EQUAL_STRING("20230615T090000", ics.events[0]->dtstart);
    TEST_ASSERT_EQUAL_STRING("20230615T100000", ics.events[0]->dtend);
    TEST_ASSERT_EQUAL_STRING("Event 2", ics.events[1]->summary);
    TEST_ASSERT_EQUAL_STRING("20230616T090000", ics.events[1]->dtstart);
    TEST_ASSERT_EQUAL_STRING("20230616T100000", ics.events[1]->dtend);

    freeIcs(&ics);
}
void test_parse_should_handle_multiple_calls_split_at_timestamp(void) {
    ics_t ics;
    initIcs(&ics);

    const char *data1 = "BEGIN:VEVENT\r\nSUMMARY:Event 1\r\nDTSTART:20230615T";
    const char *data2 = "090000\r\nDTEND:20230615T100000\r\nEND:VEVENT\r\nBEGIN:VEVENT\r\nSUMMARY:Event 2\r\nDTSTART:20230616T090000\r\nDTEND:20230616T100000\r\nEND:VEVENT\r\n";

    event_t mock_event1;
    mock_event1.summary = strdup("Event 1");
    mock_event1.dtstart = strdup("20230615T090000");
    mock_event1.dtend = strdup("20230615T100000");
    mock_event1.tstart = getTimeStamp(mock_event1.dtstart);
    mock_event1.tend = getTimeStamp(mock_event1.dtend);

    event_t mock_event2;
    mock_event2.summary = strdup("Event 2");
    mock_event2.dtstart = strdup("20230616T090000");
    mock_event2.dtend = strdup("20230616T100000");
    mock_event2.tstart = getTimeStamp(mock_event2.dtstart);
    mock_event2.tend = getTimeStamp(mock_event2.dtend);

    // Initialize buffer with data1 and call getEvent
    updateBuffer_Expect(data1);
    getEvent_ExpectAndReturn(&mock_event1);
    getEvent_ExpectAndReturn(NULL);

    // Initialize buffer with data2 and call getEvent
    updateBuffer_Expect(data2);
    getEvent_ExpectAndReturn(&mock_event2);
    getEvent_ExpectAndReturn(NULL);
    freeEvent_Ignore();
    freeEventMembers_Ignore();
    resetGetEvent_Ignore();

    parseIcs(&ics, data1);
    size_t count = parseIcs(&ics, data2);

    TEST_ASSERT_EQUAL(2, count);
    TEST_ASSERT_EQUAL_STRING("Event 1", ics.events[0]->summary);
    TEST_ASSERT_EQUAL_STRING("20230615T090000", ics.events[0]->dtstart);
    TEST_ASSERT_EQUAL_STRING("20230615T100000", ics.events[0]->dtend);
    TEST_ASSERT_EQUAL_STRING("Event 2", ics.events[1]->summary);
    TEST_ASSERT_EQUAL_STRING("20230616T090000", ics.events[1]->dtstart);
    TEST_ASSERT_EQUAL_STRING("20230616T100000", ics.events[1]->dtend);

    freeIcs(&ics);
}

#endif
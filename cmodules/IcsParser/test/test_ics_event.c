#ifdef TEST

#include "unity.h"
#include <stdlib.h>
#include "ics_event.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_getEvent_normal_case(void) {
    const char *data = "BEGIN:VEVENT\r\nSUMMARY:Meeting with John\r\nDTSTART:20230615T090000\r\nDTEND:20230615T100000\r\nEND:VEVENT\r\n";
    const char *next;
    event_t event = getEvent(data, &next);

    TEST_ASSERT_NOT_NULL(event.summary);
    TEST_ASSERT_NOT_NULL(event.dtstart);
    TEST_ASSERT_NOT_NULL(event.dtend);
    TEST_ASSERT_EQUAL_STRING("Meeting with John", event.summary);
    TEST_ASSERT_EQUAL_STRING("20230615T090000", event.dtstart);
    TEST_ASSERT_EQUAL_STRING("20230615T100000", event.dtend);

    free(event.summary);
    free(event.dtstart);
    free(event.dtend);
}

void test_getEvent_event_not_found(void) {
    const char *data = "SUMMARY:Meeting with John\r\nDTSTART:20230615T090000\r\n";
    const char *next;
    event_t event = getEvent(data, &next);

    TEST_ASSERT_NULL(event.summary);
    TEST_ASSERT_NULL(event.dtstart);
    TEST_ASSERT_NULL(event.dtend);
    TEST_ASSERT_NULL(next);
}

void test_getEvent_incomplete_event(void) {
    const char *data = "BEGIN:VEVENT\r\nSUMMARY:Meeting with John\r\nDTSTART:20230615T090000\r\n";
    const char *next;
    event_t event = getEvent(data, &next);

    TEST_ASSERT_NULL(event.summary);
    TEST_ASSERT_NULL(event.dtstart);
    TEST_ASSERT_NULL(event.dtend);
    TEST_ASSERT_EQUAL_PTR(data, next);
}

void test_getEvent_multiple_events(void) {
    const char *data = "BEGIN:VEVENT\r\nSUMMARY:Event 1\r\nDTSTART:20230615T090000\r\nDTEND:20230615T100000\r\nEND:VEVENT\r\nBEGIN:VEVENT\r\nSUMMARY:Event 2\r\nDTSTART:20230616T090000\r\nDTEND:20230616T100000\r\nEND:VEVENT\r\n";
    const char *next;
    event_t event = getEvent(data, &next);

    TEST_ASSERT_NOT_NULL(event.summary);
    TEST_ASSERT_NOT_NULL(event.dtstart);
    TEST_ASSERT_NOT_NULL(event.dtend);
    TEST_ASSERT_EQUAL_STRING("Event 1", event.summary);
    TEST_ASSERT_EQUAL_STRING("20230615T090000", event.dtstart);
    TEST_ASSERT_EQUAL_STRING("20230615T100000", event.dtend);

    free(event.summary);
    free(event.dtstart);
    free(event.dtend);

    event = getEvent(next, &next);

    TEST_ASSERT_NOT_NULL(event.summary);
    TEST_ASSERT_NOT_NULL(event.dtstart);
    TEST_ASSERT_NOT_NULL(event.dtend);
    TEST_ASSERT_EQUAL_STRING("Event 2", event.summary);
    TEST_ASSERT_EQUAL_STRING("20230616T090000", event.dtstart);
    TEST_ASSERT_EQUAL_STRING("20230616T100000", event.dtend);

    free(event.summary);
    free(event.dtstart);
    free(event.dtend);
}

void test_getEvent_missing_properties(void) {
    const char *data = "BEGIN:VEVENT\r\nDTSTART:20230615T090000\r\nEND:VEVENT\r\n";
    const char *next;
    event_t event = getEvent(data, &next);

    TEST_ASSERT_NULL(event.summary);
    TEST_ASSERT_NOT_NULL(event.dtstart);
    TEST_ASSERT_NULL(event.dtend);
    TEST_ASSERT_EQUAL_STRING("20230615T090000", event.dtstart);

    free(event.dtstart);
}

#endif // TEST

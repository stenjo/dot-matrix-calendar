#ifdef TEST

#include "unity.h"
#include <stdlib.h>
#include "ics_utils.h"
#include "ics_event.h"

void setUp(void) {
    resetGetEvent();
}

void tearDown(void) {
    // Tear down code if needed
}

void test_getEvent_normal_case(void) {
    const char *data = "BEGIN:VEVENT\r\nSUMMARY:Meeting with John\r\nDTSTART:20230615T090000\r\nDTEND:20230615T100000\r\nEND:VEVENT\r\n";
    updateBuffer(data);
    event_t * event = getEvent();

    TEST_ASSERT_NOT_NULL(event->summary);
    TEST_ASSERT_NOT_NULL(event->dtstart);
    TEST_ASSERT_NOT_NULL(event->dtend);
    TEST_ASSERT_EQUAL_STRING("Meeting with John", event->summary);
    TEST_ASSERT_EQUAL_STRING("20230615T090000", event->dtstart);
    TEST_ASSERT_EQUAL_STRING("20230615T100000", event->dtend);

    freeEvent(event);
}

void test_getEvent_event_not_found(void) {
    const char *data = "SUMMARY:Meeting with John\r\nDTSTART:20230615T090000\r\n";
    updateBuffer(data);
    const event_t *event = getEvent();

    TEST_ASSERT_NULL(event->summary);
    TEST_ASSERT_NULL(event->dtstart);
    TEST_ASSERT_NULL(event->dtend);
}

void test_getEvent_incomplete_event(void) {
    const char *data = "BEGIN:VEVENT\r\nSUMMARY:Meeting with John\r\nDTSTART:20230615T090000\r\n";
    updateBuffer(data);
    const event_t *event = getEvent();

    TEST_ASSERT_NULL(event->summary);
    TEST_ASSERT_NULL(event->dtstart);
    TEST_ASSERT_NULL(event->dtend);
}

void test_getEvent_multiple_events(void) {
    const char *data = "BEGIN:VEVENT\r\nSUMMARY:Event 1\r\nDTSTART:20230615T090000\r\nDTEND:20230615T100000\r\nEND:VEVENT\r\nBEGIN:VEVENT\r\nSUMMARY:Event 2\r\nDTSTART:20230616T090000\r\nDTEND:20230616T100000\r\nEND:VEVENT\r\n";
    updateBuffer(data);
    event_t *event = getEvent();

    TEST_ASSERT_NOT_NULL(event->summary);
    TEST_ASSERT_NOT_NULL(event->dtstart);
    TEST_ASSERT_NOT_NULL(event->dtend);
    TEST_ASSERT_EQUAL_STRING("Event 1", event->summary);
    TEST_ASSERT_EQUAL_STRING("20230615T090000", event->dtstart);
    TEST_ASSERT_EQUAL_STRING("20230615T100000", event->dtend);

    free(event->summary);
    free(event->dtstart);
    free(event->dtend);

    event = getEvent();

    TEST_ASSERT_NOT_NULL(event->summary);
    TEST_ASSERT_NOT_NULL(event->dtstart);
    TEST_ASSERT_NOT_NULL(event->dtend);
    TEST_ASSERT_EQUAL_STRING("Event 2", event->summary);
    TEST_ASSERT_EQUAL_STRING("20230616T090000", event->dtstart);
    TEST_ASSERT_EQUAL_STRING("20230616T100000", event->dtend);

    free(event->summary);
    free(event->dtstart);
    free(event->dtend);
}

void test_getEvent_missing_properties(void) {
    const char *data = "BEGIN:VEVENT\r\nDTSTART:20230615T090000\r\nEND:VEVENT\r\n";
    updateBuffer(data);
    event_t *event = getEvent();

    TEST_ASSERT_NULL(event->summary);
    TEST_ASSERT_NOT_NULL(event->dtstart);
    TEST_ASSERT_NULL(event->dtend);

    free(event->dtstart);
}

void test_getEvent_repeating_event(void) {
    const char *data = "BEGIN:VEVENT\r\nDTSTART;VALUE=DATE:20240116\r\nDTEND;VALUE=DATE:20240117\r\nRRULE:FREQ=WEEKLY;INTERVAL=4\r\nDTSTAMP:20240424T062905Z\r\nUID:bug3i5ig6j8v71399cqi8ascco@google.com\r\nCREATED:20240101T133755Z\r\nLAST-MODIFIED:20240408T100539Z\r\nSEQUENCE:0\r\nSTATUS:CONFIRMED\r\nSUMMARY:Restavfall\r\nTRANSP:TRANSPARENT\r\nX-APPLE-TRAVEL-ADVISORY-BEHAVIOR:AUTOMATIC\r\nBEGIN:VALARM\r\nACTION:AUDIO\r\nTRIGGER:-PT15H\r\nX-WR-ALARMUID:239590F8-F791-4A22-994D-6FB9571B8038\r\nUID:239590F8-F791-4A22-994D-6FB9571B8038\r\nATTACH;VALUE=URI:Chord\r\nX-APPLE-DEFAULT-ALARM:TRUE\r\nACKNOWLEDGED:20240408T100536Z\r\nEND:VALARM\r\nEND:VEVENT\r\n";
    updateBuffer(data);
    const event_t *event = getEvent();

    TEST_ASSERT_NOT_NULL(event->summary);
    TEST_ASSERT_NOT_NULL(event->dtstart);
    TEST_ASSERT_NOT_NULL(event->dtend);
    TEST_ASSERT_EQUAL_STRING("Restavfall", event->summary);
    TEST_ASSERT_EQUAL_STRING("20240116", event->dtstart);
    TEST_ASSERT_EQUAL_STRING("20240117", event->dtend);
    TEST_ASSERT_EQUAL_STRING("WEEKLY", event->rrule);
    TEST_ASSERT_EQUAL_STRING("4", event->interval);
}

#endif // TEST

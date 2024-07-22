#include "unity.h"
#include "ics_event.h"
#include <stdlib.h>
#include <string.h>
#include "ics_utils.h"

void setUp(void) {
    // Set up code here, if needed.
}

void tearDown(void) {
    // Clean up code here, if needed.
}

void test_copyMember_Should_ReturnNULL_When_SourceIsNULL(void) {
    char const *result = copyMember(NULL);
    TEST_ASSERT_NULL(result);
}

void test_copyMember_Should_CopyString_When_SourceIsValid(void) {
    const char *source = "Test String";
    char *result = copyMember(source);
    
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING(source, result);
    
    free(result);
}

void test_cloneEvent_Should_ReturnNULL_When_ExistingIsNULL(void) {
    event_t const *result = cloneEvent(NULL);
    TEST_ASSERT_NULL(result);
}

void test_cloneEvent_Should_CloneEventProperly(void) {
    event_t existing = {
        .summary = "Meeting",
        .dtstart = "2024-07-22T10:00:00",
        .dtend = "2024-07-22T11:00:00",
        .rrule = "FREQ=WEEKLY",
        .interval = "1",
        .uid = "12345"
    };

    event_t *result = cloneEvent(&existing);

    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_NOT_NULL(result->summary);
    TEST_ASSERT_NOT_NULL(result->dtstart);
    TEST_ASSERT_NOT_NULL(result->dtend);
    TEST_ASSERT_NOT_NULL(result->rrule);
    TEST_ASSERT_NOT_NULL(result->interval);
    TEST_ASSERT_NOT_NULL(result->uid);

    TEST_ASSERT_EQUAL_STRING(existing.summary, result->summary);
    TEST_ASSERT_EQUAL_STRING(existing.dtstart, result->dtstart);
    TEST_ASSERT_EQUAL_STRING(existing.dtend, result->dtend);
    TEST_ASSERT_EQUAL_STRING(existing.rrule, result->rrule);
    TEST_ASSERT_EQUAL_STRING(existing.interval, result->interval);
    TEST_ASSERT_EQUAL_STRING(existing.uid, result->uid);

    free(result->summary);
    free(result->dtstart);
    free(result->dtend);
    free(result->rrule);
    free(result->interval);
    free(result->uid);
    free(result);
}

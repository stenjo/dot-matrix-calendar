#ifdef TEST
#include <stdlib.h>
#include <string.h>
#include "unity.h"

#include "ics_event.h"

void setUp(void) {
    // Set up code if needed
}

void tearDown(void) {
    // Tear down code if needed
}

void test_extract_property_normal_case(void) {
    const char *data = "SUMMARY:Meeting with John\r\nDTSTART:20230615T090000\r\n";
    char *result = extract_property(data, "SUMMARY:");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("Meeting with John", result);
    free(result);
}

void test_extract_property_not_found(void) {
    const char *data = "DTSTART:20230615T090000\r\n";
    char *result = extract_property(data, "SUMMARY:");
    TEST_ASSERT_NULL(result);
}

void test_extract_property_value_ends_abruptly(void) {
    const char *data = "SUMMARY:Meeting with John\r\nDTSTART:20230615T090000";
    char *result = extract_property(data, "SUMMARY:");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("Meeting with John", result);
    free(result);
}

void test_extract_property_empty_value(void) {
    const char *data = "SUMMARY:\r\nDTSTART:20230615T090000\r\n";
    char *result = extract_property(data, "SUMMARY:");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("", result);
    free(result);
}

void test_extract_property_multiple_properties(void) {
    const char *data = "SUMMARY:Meeting with John\r\nSUMMARY:Second Meeting\r\n";
    char *result = extract_property(data, "SUMMARY:");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("Meeting with John", result);
    free(result);
}

#endif // TEST

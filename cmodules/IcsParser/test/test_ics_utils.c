#ifdef TEST

#include "unity.h"

#include "ics_utils.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_parse_date_string_normal_case(void) {
    const char *date_string = "20230615T090000";
    struct tm tm_result = {0};
    bool result = parse_date_string(date_string, &tm_result);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(2023 - 1900, tm_result.tm_year);
    TEST_ASSERT_EQUAL_INT(6 - 1, tm_result.tm_mon);
    TEST_ASSERT_EQUAL_INT(15, tm_result.tm_mday);
    TEST_ASSERT_EQUAL_INT(9, tm_result.tm_hour);
    TEST_ASSERT_EQUAL_INT(0, tm_result.tm_min);
    TEST_ASSERT_EQUAL_INT(0, tm_result.tm_sec);
}

void test_parse_date_string_invalid_format(void) {
    const char *date_string = "2023/06/15 09:00:00";
    struct tm tm_result = {0};
    bool result = parse_date_string(date_string, &tm_result);
    TEST_ASSERT_FALSE(result);
}

void test_parse_date_string_both_format(void) {
    const char *time_string = "20230615T000000Z";
    const char *date_string = "20230615";
    struct tm tm_time = {0};
    struct tm tm_date = {0};
    bool result = parse_date_string(date_string, &tm_date);
    TEST_ASSERT_TRUE(result);
    result = parse_date_string(time_string, &tm_time);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(mktime(&tm_time), mktime(&tm_date));
}

void test_parse_date_string_valid_format(void) {
    const char *date_string = "20240420";
    struct tm tm_result = {0};
    bool result = parse_date_string(date_string, &tm_result);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(2024 - 1900, tm_result.tm_year);
    TEST_ASSERT_EQUAL_INT(4 - 1, tm_result.tm_mon);
    TEST_ASSERT_EQUAL_INT(20, tm_result.tm_mday);
    TEST_ASSERT_EQUAL_INT(0, tm_result.tm_hour);
    TEST_ASSERT_EQUAL_INT(0, tm_result.tm_min);
    TEST_ASSERT_EQUAL_INT(0, tm_result.tm_sec);
}

void test_parse_date_string_empty_string(void) {
    const char *date_string = "";
    struct tm tm_result = {0};
    bool result = parse_date_string(date_string, &tm_result);
    TEST_ASSERT_FALSE(result);
}

void test_parse_date_string_null_pointer(void) {
    struct tm tm_result = {0};
    bool result = parse_date_string(NULL, &tm_result);
    TEST_ASSERT_FALSE(result);
}

void test_parse_date_string_edge_case_min_date(void) {
    const char *date_string = "19000101T000000";
    struct tm tm_result = {0};
    bool result = parse_date_string(date_string, &tm_result);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(1900 - 1900, tm_result.tm_year);
    TEST_ASSERT_EQUAL_INT(1 - 1, tm_result.tm_mon);
    TEST_ASSERT_EQUAL_INT(1, tm_result.tm_mday);
    TEST_ASSERT_EQUAL_INT(0, tm_result.tm_hour);
    TEST_ASSERT_EQUAL_INT(0, tm_result.tm_min);
    TEST_ASSERT_EQUAL_INT(0, tm_result.tm_sec);
}

void test_parse_date_string_edge_case_max_date(void) {
    const char *date_string = "99991231T235959";
    struct tm tm_result = {0};
    bool result = parse_date_string(date_string, &tm_result);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL_INT(9999 - 1900, tm_result.tm_year);
    TEST_ASSERT_EQUAL_INT(12 - 1, tm_result.tm_mon);
    TEST_ASSERT_EQUAL_INT(31, tm_result.tm_mday);
    TEST_ASSERT_EQUAL_INT(23, tm_result.tm_hour);
    TEST_ASSERT_EQUAL_INT(59, tm_result.tm_min);
    TEST_ASSERT_EQUAL_INT(59, tm_result.tm_sec);
}

#endif // TEST

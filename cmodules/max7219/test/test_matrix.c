#include "../src/matrix.h"
#include "../max7219.h"
#include "unity.h"


void setUp(void)
{
}

void tearDown(void)
{
}

void test_IceBreaker(void)
{
    TEST_ASSERT_EQUAL(3,3);
}

void test_textLengthInPixels(void) {
    max7219_t dev;
    char * textToTest = "0123";
    int expectedLength = 23;

    int length = textLength(&dev, textToTest);

    TEST_ASSERT_EQUAL(length, expectedLength);

}
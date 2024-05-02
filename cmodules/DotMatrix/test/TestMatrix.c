#include "unity.h"
#include "Mockmax7219.h"
#include "Mockesp_timer.h"
#include "sdkconfig.h"
#include "matrix.h"
#include <string.h>
#include <stdio.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void test_IceBreaker(void)
{
    char * textToTest = "ÆøÅ";
    TEST_ASSERT_EQUAL(195, (uint8_t)(textToTest[0]));
    TEST_ASSERT_EQUAL(134, (uint8_t)(textToTest[1]));

    TEST_ASSERT_EQUAL(6, strlen(textToTest));

    SpecialCharInfo ci;
    ci = getSpecialCharInfo(textToTest[1]);
    TEST_ASSERT_EQUAL(6, ci.width);
    ci = getSpecialCharInfo(textToTest[3]);
    TEST_ASSERT_EQUAL(5, ci.width);
    ci = getSpecialCharInfo(textToTest[5]);
    TEST_ASSERT_EQUAL(5, ci.width);
}

void test_textLengthInPixels_0123(void) {

    max7219_t dev;
    char * textToTest = "0123";
    int expectedLength = 21;

    int length = textLength(&dev, textToTest);

    TEST_ASSERT_EQUAL(expectedLength, length);

}

void test_textLengthInPixels_AEoeAA(void) {

    max7219_t dev;
    char * textToTest = "ÆøÅ";
    int expectedLength = 18;

    int length = textLength(&dev, textToTest);

    TEST_ASSERT_EQUAL(expectedLength, length);

}

void test_copyText_shortSimpleString(void) {
    
    max7219_t dev;
    dev.cascade_size = 8;
    memset(dev.frameBuffer, 0, 8*dev.cascade_size);

    char * textToTest = "ÆøÅ";
    int length = 18;
    uint8_t expected[]={
        0b00000000,
        0b01111110,0b00001001,0b00001001,0b01111111,0b01001001,0b01001001,
        0b00000000,
        0b01011000,0b00100100,0b01010100,0b01001000,0b00110100,
        0b00000000,
        0b01111000,0b00010100,0b00010101,0b00010100,0b01111000,0b00000000};
    max7219_set_digit_IgnoreAndReturn(0);

    copyText(&dev, textToTest, false);
    uint8_t* actual = dev.frameBuffer;

    TEST_PRINTF("frameBuffer(%d): \n", sizeof(expected));
    for (size_t i = 0; i < sizeof(expected); i++)
    {
        TEST_PRINTF("%X ", dev.frameBuffer[i]);
    }
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, dev.frameBuffer, sizeof(expected));
}
// void test_textLengthInPixels(char* textToTest, int expectedLength) {

//     max7219_t dev;

//     int length = textLength(&dev, textToTest);

//     TEST_ASSERT_EQUAL(expectedLength, length);

// }
// TEST_CASE("0123", 21)




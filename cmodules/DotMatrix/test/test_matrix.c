#include "unity.h"
// #include "Mockmax7219.h"
#include "Mockdriver.h"
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

    int length = textLength(textToTest);

    TEST_ASSERT_EQUAL(expectedLength, length);

}

void test_textLengthInPixels_AEoeAA(void) {

    max7219_t dev;
    char * textToTest = "ÆøÅ";
    int expectedLength = 18;

    int length = textLength(textToTest);

    TEST_ASSERT_EQUAL(expectedLength, length);

}

void test_copyText_WhenTextIsSpecialChars_UpdateFrameBufferWithPattern(void) {
    
    max7219_t dev;
    dev.cascade_size = 8;
    memset(dev.frameBuffer, 0, 8*dev.cascade_size);

    char * textToTest = "ÆøÅ";
    int length = 18;
    uint8_t expected[]={
        0b01111110,0b00001001,0b00001001,0b01111111,0b01001001,0b01001001,
        0b00000000,
        0b01011000,0b00100100,0b01010100,0b01001000,0b00110100,
        0b00000000,
        0b01111000,0b00010100,0b00010101,0b00010100,0b01111000,
        0b00000000};
    display_set_segment_Ignore();

    clear(&dev);
    copyText(&dev, textToTest, false);
    uint8_t* actual = dev.frameBuffer;

    // TEST_PRINTF("frameBuffer(%d): \n", sizeof(expected));
    // for (size_t i = 0; i < sizeof(expected); i++)
    // {
    //     TEST_PRINTF("%X ", dev.frameBuffer[i]);
    // }
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, dev.frameBuffer, sizeof(expected));
}
void test_copyText_WhenTextIsRegularChars_UpdateFrameBufferWithPattern(void) {
    
    max7219_t dev;
    dev.cascade_size = 8;
    memset(dev.frameBuffer, 0, 8*dev.cascade_size);

    char * textToTest = "abc";
    int length = 18;
    uint8_t expected[]={
        //0x20,0x54,0x54,0x54,0x78,
        0b00100000,0b01010100,0b01010100,0b01010100,0b01111000,
        0b00000000,
        //0x7F 0x28 0x44 0x44 0x38
        0b01111111,0b00101000,0b01000100,0b01000100,0b00111000,
        0b00000000,
        //0x38 0x44 0x44 0x44 0x28
        0b00111000,0b01000100,0b01000100,0b01000100,0b00101000,
        0b00000000
        };
    display_set_segment_Ignore();

    copyText(&dev, textToTest, false);
    uint8_t* actual = dev.frameBuffer;

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, dev.frameBuffer, sizeof(expected));
}

void test_copyText_WhenTextIsVariableWidthChars_UpdateFrameBufferWithPattern(void) {
    
    max7219_t dev;
    dev.cascade_size = 8;
    memset(dev.frameBuffer, 0, 8*dev.cascade_size);

    char * textToTest = "hijk";
    uint8_t expected[]={
        0b01111111,0b00000100,0b00000100,0b00000100,0b01111000, /* 104 = h */
        0b00000000,
        0b01000100,0b01111101,0b01000000,                       /* 105 = i */
        0b00000000,
        0b01000000,0b10000000,0b10000100,0b01111101,            /* 106 = j */
        0b00000000,
        0b01111111,0b00010000,0b00010000,0b00101000,0b01000100, /* 107 = k */
        0b00000000
        };
    display_set_segment_Ignore();

    copyText(&dev, textToTest, false);
    uint8_t* actual = dev.frameBuffer;

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, dev.frameBuffer, sizeof(expected));
}

void test_copyText_WhenShortTextIsCentered_AddPaddingEachSide(void) {
    
    max7219_t dev;
    dev.cascade_size = 4;
    memset(dev.frameBuffer, 0, 8*dev.cascade_size);

    char * textToTest = "O";
    uint8_t expected[]={
        0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
        0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
        0b00111110,0b01000001,0b01000001,0b01000001,0b00111110, /* O */
        0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
        0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000
        };
    display_set_segment_Ignore();

    copyText(&dev, textToTest, true);
    uint8_t* actual = dev.frameBuffer;

    // TEST_PRINTF("\n%b %b %b %b %b %b %b %b %b %b %b %b %b %b %b %b", actual[0], actual[1], actual[2], actual[3], actual[4], actual[5], actual[6], actual[7], actual[8], actual[9], actual[10], actual[11], actual[12], actual[13], actual[14], actual[15] );
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, dev.frameBuffer, sizeof(expected));
}

void test_copyText_WhenLongTextIsCentered_AddNoPaddingAndStartWithFirst(void) {
    
    max7219_t dev;
    dev.cascade_size = 2;
    memset(dev.frameBuffer, 0, 8*dev.cascade_size);

    char * textToTest = "1234";
    uint8_t expected[]={
        0b01000010,0b01111111,0b01000000,                       /* 049 = 1 */
        0b00000000,
        0b01000010,0b01100001,0b01010001,0b01001001,0b01000110, /* 050 = 2 */
        0b00000000,
        0b00100001,0b01000001,0b01000101,0b01001011,0b00110001, /* 051 = 3 */
        0b00000000
        };
    display_set_segment_Ignore();

    copyText(&dev, textToTest, true);
    uint8_t* actual = dev.frameBuffer;

    // TEST_PRINTF("\n%b %b %b %b %b %b %b %b %b %b %b %b %b %b %b %b", actual[0], actual[1], actual[2], actual[3], actual[4], actual[5], actual[6], actual[7], actual[8], actual[9], actual[10], actual[11], actual[12], actual[13], actual[14], actual[15] );
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, dev.frameBuffer, sizeof(expected));
}

// TEST_RANGE(["0123"], [21])
// void test_textLengthInPixels(char* textToTest, int expectedLength) {

//     max7219_t dev;

//     int length = textLength(&dev, textToTest);

//     TEST_ASSERT_EQUAL(expectedLength, length);

// }




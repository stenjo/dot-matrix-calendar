#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "max7219.h"

typedef struct {
    uint16_t offset;
    uint8_t width;
} SpecialCharInfo;


/**
 * @brief Copy (print) the text in display descriptor to the buffer and print.
 *
 * @param dev Display descriptor
 * @return void
 */
void copyText(max7219_t * dev, const char * text, bool center);

/**
 * @brief Shift buffer one pixel left, filling in from text string in descriptor
 *
 * @param dev Display descriptor
 * @return True if all text is scrolled out to the left, False otherwise
 */
bool scrollBuffer(max7219_t * dev);

/**
 * @brief Print the buffer in display descriptor to the display
 *
 * @param dev Display descriptor
 * @return void
 */
void printBuffer(max7219_t * dev);


/**
 * @brief Get the pixel length of a text string displayed on a cascaded 8x8 Led Matrix 
 *
 * @param dev Display descriptor
 * @param text null-terminated string to calculate
 * @return Length in number of pixels from first to last including
 */
size_t textLength(const char * text);

SpecialCharInfo getSpecialCharInfo(uint8_t chr);


/**
 * @brief Draw 64-bit image on 8x8 matrix
 *
 * @param dev Display descriptor
 * @param pos Start digit
 * @param image 64-bit buffer with image data
 * @return void
 */
void drawBlock(max7219_t *dev, uint8_t pos, const void *image);

/**
 * @brief Clear the frame buffer
 *
 * @param dev Display descriptor
 * @return void
 */
void clear(max7219_t *dev);

/**
 * @brief Scroll the text from marquee() left by one pixel and show. 
 * Timer limited to set the scroll speed constant. Call as often as possible.
 *
 * @param dev Display descriptor
 * @param wrap default False. Set to true to start scrolling text again when done.
 * @return True if all text is scrolled left and empty display. False otherwise
 */
bool scroll(max7219_t *dev, bool wrap);


/**
 * @brief Write text to the display for scrolling. Scrolling is done 
 * through the scroll() method
 *
 * @param dev Display descriptor
 * @param text Null-terminated string to write
 * @return void
 */
void marquee(max7219_t *dev, const char *text);

/**
 * @brief Write text to the display and show
 *
 * @param dev Display descriptor
 * @param text Null-terminated string to write
 * @param centered Center text on available display
 * @return void
 */
void matrixWrite(max7219_t *dev, const char *text, bool centered);

#endif // _MATRIX_H_

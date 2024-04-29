#ifndef _MATRIX_H_
#define _MATRIX_H_

// #include "max7219.h"

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
int textLength(max7219_t * dev, const char * text);

#endif // _MATRIX_H_
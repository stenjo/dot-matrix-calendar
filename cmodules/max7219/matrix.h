#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "max7219.h"

void nextChar(max7219_t *dev);
void nextCol(max7219_t *dev, uint8_t w);
void writeCol(max7219_t *dev);
void copyText(max7219_t * dev, const char * text);


#endif // _MATRIX_H_
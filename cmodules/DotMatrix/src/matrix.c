#include "matrix.h"
#include <string.h>
#include <stdint.h>
// #include "max7219.h"
#include "driver.h"
#include <esp_timer.h> // Include the header for ESP timer functions
#if defined(ESP32)
#elif defined(ESP8266)
#include <esp8266/spi.h>
#else

#endif

/*
 *  A very efficient and versatile, no-whitespace marquee example written by James Gohl 20.03.2019.
 *    - Using hardware SPI driver by Bartosz Bielawski.
 *      
 *  This example features:
 *  
 *    - Font character definitions are "bunched" together in a one-dimensional array
 *        and addressed using indexing.  The array is stored in program space
 *        freeing up RAM and saving a significant amount of space compared to
 *        other examples.
 *    - Whitespace in characters is ignored giving a cleaner appearance,
 *        character spacing is specified in nextCol().
 *    - Scrolling is performed at driver level; shifting the buffer contents
 *        left, while writing one column from the current character to the
 *        right-side of the display.
 *    - Timing is performed using elapsed milli seconds, to avoid blocking on other
 *        functions you might implement.
 *    - If you would like to draw your own fonts using my LEDMatrixPainter, 
 *        you can get it here (https://github.com/jamesgohl/LEDMatrixPainter).
 *    
 *  Wiring for Arduino UNO:
 *    
 *    MAX7219 MODULE(S)        ARDUINO UNO
 *     +-------------+
 *     /             |
 *     \         CLK --------- SCK  (PIN 13)
 *     /          CS --------- CS   (PIN 9)
 *     \         DIN --------- MOSI (PIN 11)
 *     /         GND --------- GND
 *     \         VCC --------- 5V
 *     /             |
 *     +-------------+
 *     
 *  Do NOT use the SS or MISO pin on your Arduino.
 *    
 *  For other models, check here for SPI reference: 
 *  
 *    https://www.arduino.cc/en/Reference/SPI
 *    
 */
// Set the Chip Select (CS) pin for the led matrix
// const uint8_t LEDMATRIX_CS_PIN = 9;
// Set the number of modules you have daisy-chained together.
// const uint8_t LEDMATRIX_SEGMENTS = 8;
// Width of entire display, where 8 is the number of pixels on a single module.
// const uint16_t LEDMATRIX_WIDTH    = LEDMATRIX_SEGMENTS * 8;

/*
 *  Initialise LEDMatrixDriver
 *    Since there are a lot of variants of led matrix modules out there,
 *    you can invert the column / row addressing per led segment by adding
 *    up the flags specified in argument 3, here are the options:
 *    
 *        INVERT_SEGMENT_X
 *        INVERT_DISPLAY_X
 *        INVERT_Y
 *      
 */
// LEDMatrixDriver led(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN, LEDMatrixDriver::INVERT_SEGMENT_X | LEDMatrixDriver::INVERT_Y);

/*
 *  Scroll Delay is performed using a tick count, 
 *    this ensures scroll rate is closer to the specified delay value
 *    instead of using delay(), which adds to the processing delay.
 *    
 *  Set SCROLL_DELAY to 0 to see the efficiency of this example.
 */
// const uint16_t SCROLL_DELAY = 30;
// No need to alter this.
// uint64_t marqueeDelayTimestamp = 0;

/*
 *  Font is derived from "5 by 7 Regular" by Peter Weigel with a few
 *  modifications to clean-up.
 *
 *  Stored in flash instead of RAM, for more info on this visit:
 *  //https://www.arduino.cc/reference/en/language/variables/utilities/progmem/
 *  
 *  The first byte per chatacter signifies the width in decimal form, 
 *  the rest define the pixels for each column in the char. They are
 *  in bit form for easy editing.
 *  
 *  We store a font_index[] since our characters are bunched together in a one-
 *  dimensional array, if you make any changes to a char width, all of the 
 *  indices thereafter will need to be increased, use a simple for-loop to 
 *  re-calculate and output to serial:
 *  
 *    Serial.begin(9600);
 *    int cur_idx = 0;
 *    for(int i = 0; i < 94; i++)
 *    {
 *      Serial.print(cur_idx);
 *      Serial.print(",");
 *      Serial.flush();
 *      cur_idx += pgm_read_byte(&(font[cur_idx])) + 1;
 *    }
 *  
 */
static const uint8_t font[] = {
  2,0b00000000,0b00000000,                                  /* 032 =   */
  2,0b01101111,0b01101111,                                  /* 033 = ! */
  3,0b00000011,0b00000000,0b00000011,                       /* 034 = " */
  5,0b00010100,0b01111111,0b00010100,0b01111111,0b00010100, /* 035 = # */
  5,0b00100100,0b00101010,0b01111111,0b00101010,0b00010010, /* 036 = $ */
  5,0b00100011,0b00010011,0b00001000,0b01100100,0b01100010, /* 037 = % */
  5,0b00110100,0b01001010,0b01001010,0b00110100,0b01010000, /* 038 = & */
  1,0b00000011,                                             /* 039 = ' */
  3,0b00011100,0b00100010,0b01000001,                       /* 040 = ( */
  3,0b01000001,0b00100010,0b00011100,                       /* 041 = ) */
  3,0b00000101,0b00000010,0b00000101,                       /* 042 = * */
  5,0b00001000,0b00001000,0b00111110,0b00001000,0b00001000, /* 043 = + */
  2,0b11100000,0b01100000,                                  /* 044 = , */
  5,0b00001000,0b00001000,0b00001000,0b00001000,0b00001000, /* 045 = - */
  2,0b01100000,0b01100000,                                  /* 046 = . */
  5,0b01000000,0b00110000,0b00001000,0b00000110,0b00000001, /* 047 = / */
  5,0b00111110,0b01010001,0b01001001,0b01000101,0b00111110, /* 048 = 0 */
  3,0b01000010,0b01111111,0b01000000,                       /* 049 = 1 */
  5,0b01000010,0b01100001,0b01010001,0b01001001,0b01000110, /* 050 = 2 */
  5,0b00100001,0b01000001,0b01000101,0b01001011,0b00110001, /* 051 = 3 */
  5,0b00011000,0b00010100,0b00010010,0b01111111,0b00010000, /* 052 = 4 */
  5,0b00100111,0b01000101,0b01000101,0b01000101,0b00111001, /* 053 = 5 */
  5,0b00111100,0b01001010,0b01001001,0b01001001,0b00110000, /* 054 = 6 */
  5,0b00000001,0b00000001,0b01111001,0b00000101,0b00000011, /* 055 = 7 */
  5,0b00110110,0b01001001,0b01001001,0b01001001,0b00110110, /* 056 = 8 */
  5,0b00000110,0b01001001,0b01001001,0b00101001,0b00011110, /* 057 = 9 */
  2,0b00110110,0b00110110,                                  /* 058 = : */
  2,0b01110110,0b00110110,                                  /* 059 = ; */
  4,0b00001000,0b00010100,0b00100010,0b01000001,            /* 060 = < */
  5,0b00010100,0b00010100,0b00010100,0b00010100,0b00010100, /* 061 = = */
  4,0b01000001,0b00100010,0b00010100,0b00001000,            /* 062 = > */
  5,0b00000010,0b00000001,0b01010001,0b00001001,0b00000110, /* 063 = ? */
  5,0b00111110,0b01000001,0b01011101,0b01010101,0b01011110, /* 064 = @ */
  5,0b01111110,0b00001001,0b00001001,0b00001001,0b01111110, /* 065 = A */
  5,0b01111111,0b01001001,0b01001001,0b01001001,0b00110110, /* 066 = B */
  5,0b00111110,0b01000001,0b01000001,0b01000001,0b00100010, /* 067 = C */
  5,0b01111111,0b01000001,0b01000001,0b01000001,0b00111110, /* 068 = D */
  5,0b01111111,0b01001001,0b01001001,0b01001001,0b01000001, /* 069 = E */
  5,0b01111111,0b00001001,0b00001001,0b00001001,0b00000001, /* 070 = F */
  5,0b00111110,0b01000001,0b01001001,0b01001001,0b01111010, /* 071 = G */
  5,0b01111111,0b00001000,0b00001000,0b00001000,0b01111111, /* 072 = H */
  5,0b01000001,0b01000001,0b01111111,0b01000001,0b01000001, /* 073 = I */
  5,0b00100000,0b01000001,0b01000001,0b00111111,0b00000001, /* 074 = J */
  5,0b01111111,0b00001000,0b00010100,0b00100010,0b01000001, /* 075 = K */
  5,0b01111111,0b01000000,0b01000000,0b01000000,0b01000000, /* 076 = L */
  5,0b01111111,0b00000010,0b00000100,0b00000010,0b01111111, /* 077 = M */
  5,0b01111111,0b00000110,0b00001000,0b00110000,0b01111111, /* 078 = N */
  5,0b00111110,0b01000001,0b01000001,0b01000001,0b00111110, /* 079 = O */
  5,0b01111111,0b00010001,0b00010001,0b00010001,0b00001110, /* 080 = P */
  5,0b00111110,0b01000001,0b01010001,0b00100001,0b01011110, /* 081 = Q */
  5,0b01111111,0b00001001,0b00011001,0b00101001,0b01000110, /* 082 = R */
  5,0b00100110,0b01001001,0b01001001,0b01001001,0b00110010, /* 083 = S */
  5,0b00000001,0b00000001,0b01111111,0b00000001,0b00000001, /* 084 = T */
  5,0b00111111,0b01000000,0b01000000,0b01000000,0b00111111, /* 085 = U */
  5,0b00011111,0b00100000,0b01000000,0b00100000,0b00011111, /* 086 = V */
  5,0b00111111,0b01000000,0b00110000,0b01000000,0b00111111, /* 087 = W */
  5,0b01100011,0b00010100,0b00001000,0b00010100,0b01100011, /* 088 = X */
  5,0b00000111,0b00001000,0b01110000,0b00001000,0b00000111, /* 089 = Y */
  5,0b01100001,0b01010001,0b01001001,0b01000101,0b01000011, /* 090 = Z */
  3,0b01111111,0b01000001,0b01000001,                       /* 091 = [ */
  5,0b00000001,0b00000110,0b00001000,0b00110000,0b01000000, /* 092 = \ */
  3,0b01000001,0b01000001,0b01111111,                       /* 093 = ] */
  5,0b00000100,0b00000010,0b00000001,0b00000010,0b00000100, /* 094 = ^ */
  5,0b01000000,0b01000000,0b01000000,0b01000000,0b01000000, /* 095 = _ */
  1,0b00000011,                                             /* 096 = ' */
  5,0b00100000,0b01010100,0b01010100,0b01010100,0b01111000, /* 097 = a */
  5,0b01111111,0b00101000,0b01000100,0b01000100,0b00111000, /* 098 = b */
  5,0b00111000,0b01000100,0b01000100,0b01000100,0b00101000, /* 099 = c */
  5,0b00111000,0b01000100,0b01000100,0b00101000,0b01111111, /* 100 = d */
  5,0b00111000,0b01010100,0b01010100,0b01010100,0b00011000, /* 101 = e */
  5,0b00000100,0b01111110,0b00000101,0b00000001,0b00000010, /* 102 = f */
  5,0b00011000,0b10100100,0b10100100,0b10100100,0b01111100, /* 103 = g */
  5,0b01111111,0b00000100,0b00000100,0b00000100,0b01111000, /* 104 = h */
  3,0b01000100,0b01111101,0b01000000,                       /* 105 = i */
  4,0b01000000,0b10000000,0b10000100,0b01111101,            /* 106 = j */
  5,0b01111111,0b00010000,0b00010000,0b00101000,0b01000100, /* 107 = k */
  3,0b01000001,0b01111111,0b01000000,                       /* 108 = l */
  5,0b01111100,0b00000100,0b01111100,0b00000100,0b01111000, /* 109 = m */
  5,0b01111100,0b00001000,0b00000100,0b00000100,0b01111000, /* 110 = n */
  5,0b00111000,0b01000100,0b01000100,0b01000100,0b00111000, /* 111 = o */
  5,0b11111100,0b00100100,0b00100100,0b00100100,0b00011000, /* 112 = p */
  5,0b00011000,0b00100100,0b00100100,0b00100100,0b11111100, /* 113 = q */
  5,0b01111100,0b00001000,0b00000100,0b00000100,0b00001000, /* 114 = r */
  5,0b01001000,0b01010100,0b01010100,0b01010100,0b00100000, /* 115 = s */
  5,0b00000100,0b00111110,0b01000100,0b01000000,0b00100000, /* 116 = t */
  5,0b00111100,0b01000000,0b01000000,0b00100000,0b01111100, /* 117 = u */
  5,0b00011100,0b00100000,0b01000000,0b00100000,0b00011100, /* 118 = v */
  5,0b00111100,0b01000000,0b00110000,0b01000000,0b00111100, /* 119 = w */
  5,0b01000100,0b00101000,0b00010000,0b00101000,0b01000100, /* 120 = x */
  5,0b00000100,0b01001000,0b00110000,0b00001000,0b00000100, /* 121 = y */
  5,0b01000100,0b01100100,0b01010100,0b01001100,0b01000100, /* 122 = z */
  3,0b00001000,0b00110110,0b01000001,                       /* 123 = { */
  1,0b01111111,                                             /* 124 = | */
  3,0b01000001,0b00110110,0b00001000,                       /* 125 = } */
  5,0b00011000,0b00000100,0b00001000,0b00010000,0b00001100, /* 126 = ~ */
  1,0b00000000,                                             /* 127 =   */
  1,0b00000000,                                             /* 128 =   */
  1,0b00000000,                                             /* 129 =   */
  1,0b00000000,                                             /* 130 =   */
  1,0b00000000,                                             /* 131 =   */
  1,0b00000000,                                             /* 132 =   */
  5,0b01111000,0b00010100,0b00010101,0b00010100,0b01111000, /* 133 = Å */
  6,0b01111110,0b00001001,0b00001001,0b01111111,0b01001001,0b01001001,   /* 134 = Æ */
  1,0b00000000,                                             /* 135 =   */
  1,0b00000000,                                             /* 136 =   */
  1,0b00000000,                                             /* 137 =   */
  1,0b00000000,                                             /* 138 =   */
  1,0b00000000,                                             /* 139 =   */
  1,0b00000000,                                             /* 140 =   */
  1,0b00000000,                                             /* 141 =   */
  1,0b00000000,                                             /* 142 =   */
  1,0b00000000,                                             /* 143 =   */
  1,0b00000000,                                             /* 144 =   */
  1,0b00000000,                                             /* 145 =   */
  1,0b00000000,                                             /* 146 =   */
  1,0b00000000,                                             /* 147 =   */
  1,0b00000000,                                             /* 148 =   */
  1,0b00000000,                                             /* 149 =   */
  1,0b00000000,                                             /* 150 =   */
  1,0b00000000,                                             /* 151 =   */
  5,0b01011110,0b00100001,0b01011001,0b01000110,0b00111101, /* 152 = Ø */
  1,0b00000000,                                             /* 153 =   */
  1,0b00000000,                                             /* 154 =   */
  1,0b00000000,                                             /* 155 =   */
  1,0b00000000,                                             /* 156 =   */
  1,0b00000000,                                             /* 157 =   */
  1,0b00000000,                                             /* 158 =   */
  1,0b00000000,                                             /* 159 =   */
  1,0b00000000,                                             /* 160 =   */
  1,0b00000000,                                             /* 161 =   */
  1,0b00000000,                                             /* 162 =   */
  1,0b00000000,                                             /* 163 =   */
  1,0b00000000,                                             /* 164 =   */
  5,0b00100000,0b01010100,0b01010101,0b01010100,0b01111000, /* 165 = å */
  5,0b01011000,0b00100100,0b01010100,0b01001000,0b00110100, /* 166 = ø */
  1,0b00000000,                                             /* 167 =   */
  1,0b00000000,                                             /* 168 =   */
  1,0b00000000,                                             /* 169 =   */
  1,0b00000000,                                             /* 170 =   */
  1,0b00000000,                                             /* 171 =   */
  1,0b00000000,                                             /* 172 =   */
  1,0b00000000,                                             /* 173 =   */
  1,0b00000000,                                             /* 174 =   */
  1,0b00000000,                                             /* 175 =   */
  1,0b00000000,                                             /* 176 =   */
  1,0b00000000,                                             /* 177 =   */
  1,0b00000000,                                             /* 178 =   */
  1,0b00000000,                                             /* 179 =   */
  1,0b00000000,                                             /* 180 =   */
  1,0b00000000,                                             /* 181 =   */
  1,0b00000000,                                             /* 182 =   */
  1,0b00000000,                                             /* 183 =   */
  6,0b00100000,0b01010100,0b01010100,0b01111100,0b01010100,0b01011000,   /* 184 = æ */
  1,0b00000000
};

static const uint16_t font_index[95] = {
  0,3,6,10,16,22,28,34,36,40,44,48,54,57,63,66,72,78,82,88,94,
  100,106,112,118,124,130,133,136,141,147,152,158,164,170,176,
  182,188,194,200,206,212,218,224,230,236,242,248,254,260,266,
  272,278,284,290,296,302,308,314,320,324,330,334,340,346,348,
  354,360,366,372,378,384,390,396,400,405,411,415,421,427,433,
  439,445,451,457,463,469,475,481,487,493,499,503,505,509
};

static const uint8_t specials[] = { // Escaped by char 195
6, 0b00100000,0b01010100,0b01010100,0b01111100,0b01010100,0b01011000,   /* 184 = æ */
5, 0b01011000,0b00100100,0b01010100,0b01001000,0b00110100,              /* 166 = ø */
5, 0b00100000,0b01010100,0b01010101,0b01010100,0b01111000,              /* 165 = å */
6, 0b01111110,0b00001001,0b00001001,0b01111111,0b01001001,0b01001001,   /* 134 = Æ */
5, 0b01011110,0b00100001,0b01011001,0b01000110,0b00111101,              /* 152 = Ø */
5, 0b01111000,0b00010100,0b00010101,0b00010100,0b01111000,0b00000000    /* 133 = Å */
};

static const uint16_t specials_map[] = {
  0,7,13,19,26,32
};

static const uint8_t ESCAPE_CHAR = 195;



void rotateImageCounterClockwise(const uint8_t* frameBuffer, uint8_t* rotatedBuffer) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (frameBuffer[i] & (1 << j)) {
                rotatedBuffer[j] |= (1 << (7 - i));
            }
        }
    }
}

SpecialCharInfo getSpecialCharInfo(uint8_t chr) {
    SpecialCharInfo info = {0, 0}; // Initialize to zero, indicating an invalid character (by default)
    switch (chr) {
        case 166: info.offset = specials_map[0]; break;
        case 184: info.offset = specials_map[1]; break;
        case 165: info.offset = specials_map[2]; break;
        case 134: info.offset = specials_map[3]; break;
        case 152: info.offset = specials_map[4]; break;
        case 133: info.offset = specials_map[5]; break;
        default: return info; // If the character is not special, we return zeros
    }
    info.width = specials[info.offset]; // Determine width using offset
    return info;
}

uint8_t getSpecials(uint8_t chr, const uint8_t col) {
    SpecialCharInfo info = getSpecialCharInfo(chr);
  
    if (col >= info.width) {
        return 0;
    }
    return specials[col + 1 + info.offset];
}

uint8_t getCharColumn(uint8_t chr, uint8_t pos) {
  uint8_t asc = chr - 32;
  uint16_t idx = font_index[asc];
  uint8_t w = font[idx];
  if (pos >= w) return 0;
  return font[pos + idx + 1];
}

void copyText(max7219_t * dev, const char * text, bool center) {
  size_t txtLen = textLength(text); // Calculate the length of the text once
  int16_t bufferIndex = 0;
  int16_t padding = 0;
  bool specialChar = false;

  if (txtLen < dev->cascade_size*8 && center) {
    padding = (dev->cascade_size*8 - txtLen)/2;
    for (int16_t i = 0; i < padding; i++) {
      dev->frameBuffer[bufferIndex++] = 0;
    }
  }

  for (size_t strIdx = 0; strIdx < strlen(text) && bufferIndex < MAX7219_MAX_CASCADE_SIZE*8; strIdx++) {
    uint8_t chr = text[strIdx];
    
    if (chr == ' ') {
      // If the character is a space, skip 2 columns
      bufferIndex += 3;
      continue;
    }
    
    if (chr == ESCAPE_CHAR) {
      // If the character is an escape character, handle it specially
      strIdx++; // Move to the escaped char
      if (strIdx < strlen(text) ) {
        chr = text[strIdx];
        specialChar = true;
      } else {
        // Error handling if ESCAPE_CHAR is the last character in the text
        break;
      }
    }
    
    // Process each column of the character
    for (int16_t charColumn = 0; ; charColumn++) {
      uint8_t col = (specialChar) 
                    ? getSpecials(text[strIdx], charColumn) 
                    : getCharColumn(chr, charColumn);

      // Exit the loop if the column data is zero (i.e., column processing is complete)
      if (col == 0 || bufferIndex >= MAX7219_MAX_CASCADE_SIZE*8) break;

      // Write to the buffer and increment the buffer index
      dev->frameBuffer[bufferIndex++] = col;
    }
    specialChar = false;
    if (bufferIndex < MAX7219_MAX_CASCADE_SIZE*8) {
      dev->frameBuffer[bufferIndex++] = 0;
    }
  }

  // clear remaining buffer
  while (bufferIndex < MAX7219_MAX_CASCADE_SIZE*8)
  {
    dev->frameBuffer[bufferIndex++] = 0;
  }
 // After filling the buffer, print it
  printBuffer(dev);
}

bool scrollBuffer(max7219_t *dev) {
    size_t bufferSize = dev->cascade_size * 8;
    size_t textLength = strlen(dev->text);

    memmove(dev->frameBuffer, dev->frameBuffer + 1, bufferSize - 1);

    if (dev->text_index >= textLength) {
        // Finished processing text, add whitespace
        dev->frameBuffer[bufferSize - 1] = 0;
        if (--dev->scroll_whitespace <= 0) {  // Decrement and check whitespace count
            if (dev->wrap_text_scroll) {
                dev->text_index = 0;  // Reset text index
                dev->col_index = 0;   // Reset column index
                dev->scroll_whitespace = bufferSize;  // Reset whitespace count
            }
            return true;  // End of scrolling
        }
    } else {
        uint8_t chr = dev->text[dev->text_index];
        uint8_t col = 0;

        if (dev->char_escaped) {
            // Fetch the column for the special character
            col = getSpecials(chr, dev->col_index);
        } else if (chr == ' ' && dev->col_index < 1) {
            dev->col_index++;
            return false; 
        } else {
            // Fetch the column for the normal character
            col = getCharColumn(chr, dev->col_index);
            // Check if the character is an escape character
            if (chr == ESCAPE_CHAR) {
                dev->char_escaped = true;
                chr = dev->text[++dev->text_index];  // Move to the next character and use as special character
                col = getSpecials(chr, dev->col_index);
            }
        }

        if (col != 0) {
            // If there is column data, add it to buffer and increment column index
            dev->frameBuffer[bufferSize - 1] = col;
            dev->col_index++;
        } else {
            // No more columns for the character, reset column index, move to next character
            dev->frameBuffer[bufferSize - 1] = 0;
            dev->col_index = 0;
            dev->text_index++;  // Increment text index to move to next character
            dev->char_escaped = false;  // Reset escape flag after processing a special character
        }
        // Reset whitespace count as long as we're displaying text
        dev->scroll_whitespace = bufferSize;
    }
    return false;  // Continue scrolling
}

void printBuffer(max7219_t * dev)
{
  size_t offs = 0;
  uint8_t buffer[8];
  for (int16_t d = 0; d < dev->cascade_size; d++)
  {
    memset(buffer, 0, 8);
    rotateImageCounterClockwise((uint8_t *)(dev->frameBuffer + d * 8 + offs), (uint8_t *)buffer);
    drawBlock(dev, d*8, buffer);
  }
}

size_t textLength(const char * text)
{
  size_t len = 0;
  for (uint16_t i = 0; i < strlen(text); i++) {
 
    if ((uint8_t)(text[i]) == ESCAPE_CHAR) {
      i++;
      SpecialCharInfo info = getSpecialCharInfo(text[i]);
      len += info.width + 1;
    }
    else {
      len += font[font_index[text[i]-32]]+1;
    }
  }
  return len-1;
}

void drawBlock(max7219_t *dev, uint8_t pos, const void *image)
{
    for (uint8_t i = pos, offs = 0; i < dev->cascade_size*8 && offs < 8; i++, offs++)
        display_set_segment(dev, i, *((uint8_t *)image + offs));
}
void clear(max7219_t *dev) {memset(dev->frameBuffer, 0, 8*dev->cascade_size);}


bool scroll(max7219_t *dev, bool wrap)
{

    uint64_t currentMillis = esp_timer_get_time() / 1000;
    dev->wrap_text_scroll = wrap;

    if ((currentMillis < dev->mrqTmstmp) || (currentMillis - dev->mrqTmstmp < dev->scroll_delay)) {
        return false;
    } 
    dev->mrqTmstmp = currentMillis + dev->scroll_delay;

    bool done = scrollBuffer(dev);
    printBuffer(dev);

    return done;
}

void marquee(max7219_t *dev, const char *text)
{
    dev->text = text;
    dev->text_index = 0;
    dev->col_index = 0;
    dev->scroll_whitespace = 0;
    dev->char_escaped = false;
    clear(dev);
    init_display(dev);
    display_clear(dev);
}

void matrixWrite(max7219_t *dev, const char *text, bool centered)
{
    // clear(dev);
    // max7219_clear(dev);

    copyText(dev, text, centered);
}

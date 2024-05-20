#ifndef MAX7219_H
#define MAX7219_H

#include <stdint.h>
#include <stdbool.h>

#if defined(ESP32)
#include <esp_log.h>
#include <esp_timer.h> // Include the header for ESP timer functions
#include <driver/spi_master.h>
#include <driver/gpio.h> // add by nopnop2002
#elif defined(ESP8266)
#include <esp_log.h>
#include <esp_timer.h> // Include the header for ESP timer functions
#include <hspi.h>
#else
#include <driver/spi_master.h>
#include <driver/gpio.h> // add by nopnop2002
#endif


#define MAX7219_MAX_CLOCK_SPEED_HZ (1000000) // 1 MHz

#define MAX7219_MAX_CASCADE_SIZE 16
#define MAX7219_MAX_BRIGHTNESS   15

#define DEFAULT_SCROLL_DELAY 30
#define DEFAULT_CASCADE_SIZE 8
#define DEFAULT_SPI_HOST        MICROPY_HW_SPI_HOST
#define DEFAULT_PIN_NUM_MOSI    MICROPY_HW_SPI1_MOSI //19
#define DEFAULT_PIN_NUM_CLK     MICROPY_HW_SPI1_SCK //18
#define DEFAULT_PIN_CS 5

//commands as defined in the datasheet
#define  ENABLE         0x0C00
// #define  DRIVER_TEST    0x0F00
#define  INTENSITY      0x0A00
#define  SCAN_LIMIT     0x0B00
#define  DECODE         0x0900

/**
 * Display descriptor
 */
typedef struct
{
    spi_device_interface_config_t spi_cfg;
    spi_device_handle_t spi_dev;
    uint8_t digits;              //!< Accessible digits in 7seg. Up to cascade_size * 8
    uint8_t cascade_size;        //!< Up to `MAX7219_MAX_CASCADE_SIZE` MAX721xx cascaded
    uint8_t flags;
    uint16_t scroll_delay;
    bool mirrored;               //!< true for horizontally mirrored displays
    bool bcd;
    bool wrap_text_scroll;
    bool char_escaped;
    // scrollDirection_t direction;
    const char * text;
    uint16_t text_index;
    uint16_t col_index;
    int16_t scroll_whitespace;
    uint64_t mrqTmstmp;
    uint8_t frameBuffer[MAX7219_MAX_CASCADE_SIZE*8];
} max7219_t;



void init_descriptor(max7219_t *dev, spi_host_device_t host, uint32_t clock_speed_hz, gpio_num_t cs_pin);
void init_display(max7219_t *dev);
void display_set_segment(max7219_t *dev, uint8_t digit, uint8_t val);
void display_clear(max7219_t *dev);



#endif // MAX7219_H

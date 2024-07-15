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
#include "soc/spi_pins.h"
#endif

#if SOC_SPI_PERIPH_NUM > 2
#define MICROPY_HW_SPI_MAX (2)
#else
#define MICROPY_HW_SPI_MAX (1)
#endif

// Default pins for SPI(id=1) aka IDF SPI2, can be overridden by a board
#ifndef MICROPY_HW_SPI1_SCK
// Use IO_MUX pins by default.
// If SPI lines are routed to other pins through GPIO matrix
// routing adds some delay and lower limit applies to SPI clk freq
#define MICROPY_HW_SPI1_SCK SPI2_IOMUX_PIN_NUM_CLK
#define MICROPY_HW_SPI1_MOSI SPI2_IOMUX_PIN_NUM_MOSI
#define MICROPY_HW_SPI1_MISO SPI2_IOMUX_PIN_NUM_MISO
#endif

// Default pins for SPI(id=2) aka IDF SPI3, can be overridden by a board
#ifndef MICROPY_HW_SPI2_SCK
#if CONFIG_IDF_TARGET_ESP32
// ESP32 has IO_MUX pins for VSPI/SPI3 lines, use them as defaults
#define MICROPY_HW_SPI2_SCK SPI3_IOMUX_PIN_NUM_CLK      // pin 18
#define MICROPY_HW_SPI2_MOSI SPI3_IOMUX_PIN_NUM_MOSI    // pin 23
#define MICROPY_HW_SPI2_MISO SPI3_IOMUX_PIN_NUM_MISO    // pin 19
#elif CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
// ESP32S2 and S3 uses GPIO matrix for SPI3 pins, no IO_MUX possible
// Set defaults to the pins used by SPI2 in Octal mode
#define MICROPY_HW_SPI2_SCK (36)
#define MICROPY_HW_SPI2_MOSI (35)
#define MICROPY_HW_SPI2_MISO (37)
#endif
#endif

#define MP_HW_SPI_MAX_XFER_BYTES (4092)
#define MP_HW_SPI_MAX_XFER_BITS (MP_HW_SPI_MAX_XFER_BYTES * 8) // Has to be an even multiple of 8

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
    /* SPI Controls*/
    spi_device_interface_config_t spi_cfg;
    spi_host_device_t host;
    uint32_t baudrate;
    uint8_t polarity;
    uint8_t phase;
    uint8_t bits;
    uint8_t firstbit;
    int8_t sck;
    int8_t mosi;
    int8_t miso;
    spi_device_handle_t spi_dev;
    enum {
        MAX7219_SPI_STATE_NONE,
        MAX7219_SPI_STATE_INIT,
        MAX7219_SPI_STATE_DEINIT
    } state;

    /* MAX7219 controls */    
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


typedef struct _max7219_spi_default_pins_t {
    union {
        int8_t array[3];
        struct {
            // Must be in enum's ARG_sck, ARG_mosi, ARG_miso, etc. order
            int8_t sck;
            int8_t mosi;
            int8_t miso;
        } pins;
    };
} spi_default_pins_t;


// Default pin mappings for the hardware SPI instances
static const spi_default_pins_t max7219_spi_default_pins[MICROPY_HW_SPI_MAX] = {
    { .pins = { .sck = MICROPY_HW_SPI1_SCK, .mosi = MICROPY_HW_SPI1_MOSI, .miso = MICROPY_HW_SPI1_MISO }},
    #ifdef MICROPY_HW_SPI2_SCK
    { .pins = { .sck = MICROPY_HW_SPI2_SCK, .mosi = MICROPY_HW_SPI2_MOSI, .miso = MICROPY_HW_SPI2_MISO }},
    #endif
};


void init_descriptor(max7219_t *dev, spi_host_device_t host, uint32_t clock_speed_hz, gpio_num_t cs_pin);
void init_display(max7219_t *dev);
void display_set_segment(max7219_t *dev, uint8_t digit, uint8_t val);
void display_clear(max7219_t *dev);



#endif // MAX7219_H

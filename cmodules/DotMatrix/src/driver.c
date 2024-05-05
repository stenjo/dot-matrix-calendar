#include "driver.h"
#include <string.h>

#if defined(ESP32)
#include <esp_log.h>
#include <esp_timer.h> // Include the header for ESP timer functions
#include <driver/spi_master.h>
#elif defined(ESP8266)
#include <esp8266/spi.h>
#else
#include <driver/spi_master.h>
#endif


#define ALL_CHIPS 0xff
#define ALL_DIGITS 8

#define REG_DIGIT_0      (1 << 8)
#define REG_DECODE_MODE  (9 << 8)
#define REG_INTENSITY    (10 << 8)
#define REG_SCAN_LIMIT   (11 << 8)
#define REG_SHUTDOWN     (12 << 8)
#define REG_DISPLAY_TEST (15 << 8)

#define VAL_CLEAR_NORMAL 0x00

// Local functions

static inline uint16_t shuffle(uint16_t val)
{
    return (val >> 8) | (val << 8);
}

static void send(max7219_t *dev, uint8_t chip, uint16_t value)
{
    uint16_t buf[MAX7219_MAX_CASCADE_SIZE] = { 0 };
    if (chip == ALL_CHIPS)
    {
        for (uint8_t i = 0; i < dev->cascade_size; i++)
            buf[i] = shuffle(value);
    }
    else buf[chip] = shuffle(value);

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = dev->cascade_size * 16;
    t.tx_buffer = buf;
    spi_device_transmit(dev->spi_dev, &t);
}

void set_shutdown_mode(max7219_t *dev, bool shutdown)
{
    send(dev, ALL_CHIPS, REG_SHUTDOWN | !shutdown);
}

void set_brightness(max7219_t *dev, uint8_t value)
{
    send(dev, ALL_CHIPS, REG_INTENSITY | value);
}

void set_decode_mode(max7219_t *dev)
{
    send(dev, ALL_CHIPS, REG_DECODE_MODE | 0);
    display_clear(dev);
}

// Public functions

void init_descriptor(max7219_t* dev, spi_host_device_t host, uint32_t clock_speed_hz, gpio_num_t cs_pin)
{
    memset(&dev->spi_cfg, 0, sizeof(dev->spi_cfg));
    dev->spi_cfg.spics_io_num = cs_pin;
    dev->spi_cfg.clock_speed_hz = clock_speed_hz;
    dev->spi_cfg.mode = 0;
    dev->spi_cfg.queue_size = 1;
    dev->spi_cfg.flags = SPI_DEVICE_NO_DUMMY;

    spi_bus_add_device(host, &dev->spi_cfg, &dev->spi_dev);
}

void init_display(max7219_t* dev)
{
    if (!dev->cascade_size || dev->cascade_size > MAX7219_MAX_CASCADE_SIZE)
    {
        printf("Invalid cascade size %d", dev->cascade_size);
        return;
    }

    uint8_t max_digits = dev->cascade_size * ALL_DIGITS;
    if (dev->digits > max_digits)
    {
        printf("Invalid digits count %d, max %d", dev->digits, max_digits);
        return;
    }
    if (!dev->digits)
        dev->digits = max_digits;

    // Shutdown all chips
    set_shutdown_mode(dev, true);
    // Disable test
    send(dev, ALL_CHIPS, REG_DISPLAY_TEST);
    // Set max scan limit
    send(dev, ALL_CHIPS, REG_SCAN_LIMIT | (ALL_DIGITS - 1));
    // Set normal decode mode & clear display
    set_decode_mode(dev);
    // Set minimal brightness
    set_brightness(dev, 0);
    // Wake up
    set_shutdown_mode(dev, false);

    // dev->frameBuffer = malloc(dev->cascade_size*8);
    display_clear(dev);
}

void display_set_segment(max7219_t* dev, uint8_t digit, uint8_t val)
{
    if (digit >= dev->digits)
    {
        printf("Invalid digit: %d", digit);
        return;
    }

    if (dev->mirrored)
        digit = dev->digits - digit - 1;

    uint8_t c = digit / ALL_DIGITS;
    uint8_t d = digit % ALL_DIGITS;

    // printf("Chip %d, digit %d val 0x%02x", c, d, val);

    send(dev, c, (REG_DIGIT_0 + ((uint16_t)d << 8)) | val);
}

void display_clear(max7219_t *dev)
{
    uint8_t val = VAL_CLEAR_NORMAL;
    for (uint8_t i = 0; i < ALL_DIGITS; i++)
        send(dev, ALL_CHIPS, (REG_DIGIT_0 + ((uint16_t)i << 8)) | val);

}





#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "esp_log.h"

#include "py/runtime.h"
#include "py/stream.h"
#include "py/mphal.h"
#include "extmod/modmachine.h"

#include "driver/spi_master.h"
#include "soc/gpio_sig_map.h"
#include "soc/spi_pins.h"
#include "matrix.h"
#include "max7219.h"

#include <mpconfigboard.h>
#if defined(ESP32)

#elif defined(ESP8266)
#include <esp8266/spi.h>
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

typedef struct _mp_dotmatrix_spi_default_pins_t {
    union {
        int8_t array[4];
        struct {
            // Must be in enum's ARG_sck, ARG_mosi, ARG_miso, etc. order
            int8_t sck;
            int8_t mosi;
            int8_t miso;
            int8_t cs;
        } pins;
    };
} mp_dotmatrix_spi_default_pins_t;

typedef struct _dotmatrix_obj_t {
    mp_obj_base_t base;
    max7219_t dev;
} dotmatrix_obj_t;

// Default pin mappings for the hardware SPI instances
static const mp_dotmatrix_spi_default_pins_t mp_dotmatrix_spi_default_pins[MICROPY_HW_SPI_MAX] = {
    { .pins = { .sck = MICROPY_HW_SPI1_SCK, .mosi = MICROPY_HW_SPI1_MOSI, .miso = MICROPY_HW_SPI1_MISO, .cs = DEFAULT_PIN_CS }},
    #ifdef MICROPY_HW_SPI2_SCK
    { .pins = { .sck = MICROPY_HW_SPI2_SCK, .mosi = MICROPY_HW_SPI2_MOSI, .miso = MICROPY_HW_SPI2_MISO, .cs = DEFAULT_PIN_CS }},
    #endif
};

// Common arguments for init() and make new
enum { ARG_modules, ARG_shift_delay, ARG_host, ARG_baudrate, ARG_polarity, ARG_phase, ARG_bits, ARG_firstbit, ARG_sck, ARG_mosi, ARG_miso, ARG_cs };
static const mp_arg_t spi_allowed_args[] = {
    { MP_QSTR_modules,      MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
    { MP_QSTR_shift_delay,  MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
    { MP_QSTR_host,       MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
    { MP_QSTR_baudrate,     MP_ARG_INT, {.u_int = -1} },
    { MP_QSTR_polarity,     MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
    { MP_QSTR_phase,        MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
    { MP_QSTR_bits,         MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
    { MP_QSTR_firstbit,     MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
    { MP_QSTR_sck,          MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
    { MP_QSTR_mosi,         MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
    { MP_QSTR_miso,         MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
    { MP_QSTR_cs,           MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
};


static void spi_deinit(max7219_t *spi_dev) {
    switch (spi_bus_remove_device(spi_dev->spi_dev)) {
        case ESP_ERR_INVALID_ARG:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("invalid configuration"));
            return;

        case ESP_ERR_INVALID_STATE:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("SPI device already freed"));
            return;
    }

    switch (spi_bus_free(spi_dev->host)) {
        case ESP_ERR_INVALID_ARG:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("invalid configuration"));
            return;

        case ESP_ERR_INVALID_STATE:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("SPI bus already freed"));
            return;
    }

    int8_t pins[4] = {spi_dev->miso, spi_dev->mosi, spi_dev->sck, spi_dev->cs};

    for (int i = 0; i < 4; i++) {
        if (pins[i] != -1) {
            esp_rom_gpio_pad_select_gpio(pins[i]);
            esp_rom_gpio_connect_out_signal(pins[i], SIG_GPIO_OUT_IDX, false, false);
            gpio_set_direction(pins[i], GPIO_MODE_INPUT);
        }
    }
}

// static void mp_dotmatrix_spi_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
//     dotmatrix_obj_t *self = MP_OBJ_TO_PTR(self_in);
//     max7219_t *dev = &(self->dev);
//     mp_printf(print, "SPI(id=%u, baudrate=%u, polarity=%u, phase=%u, bits=%u, firstbit=%u, sck=%d, mosi=%d, miso=%d)",
//         dev->host, dev->baudrate, dev->polarity,
//         dev->phase, dev->bits, dev->firstbit,
//         dev->sck, dev->mosi, dev->miso);
// }

static void mp_dotmatrix_spi_argcheck(mp_arg_val_t args[], const mp_dotmatrix_spi_default_pins_t *default_pins) {
// A non-NULL default_pins argument will trigger the "use default" behavior.
    // Replace pin args with default/current values for new vs init call, respectively
    for (int i = ARG_sck; i <= ARG_cs; i++) {
        if (args[i].u_obj == MP_OBJ_NULL) {
            args[i].u_int = default_pins ? default_pins->array[i - ARG_sck] : -2;
        } else if (args[i].u_obj == mp_const_none) {
            args[i].u_int = -1;
        } else {
            args[i].u_int = machine_pin_get_id(args[i].u_obj);
        }
    }
}

/**
 * @brief Initialize Matrix8x8 object
 * 
 * @param modules: number of 8x8 dot matrix modules cascaded. Default: 8
 * @param scroll_delay: Delay in milliseconds between shifting pixels left. Default is 30
 * @param host: 0 or 1. SPI device to use. Default: SPI2_HOST (=1)
 * @param baudrate: SPI clocking speed in Hz. Default 1000000 (1 MHz)
 * @param cs: CS pin number. Default 5
 * 
 * @returns Matrix8x8 Object initialized
*/
static mp_obj_t dotmatrix_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    dotmatrix_obj_t *self = mp_obj_malloc(dotmatrix_obj_t, type);

    max7219_t *spi = &(self->dev);

    bool changed = spi->state != MAX7219_SPI_STATE_INIT;
    esp_err_t ret;
    max7219_t old_dev = *spi;

    mp_arg_val_t args[MP_ARRAY_SIZE(spi_allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(spi_allowed_args), spi_allowed_args, args);

    const mp_int_t host = args[ARG_host].u_int;
    if (1 <= host && host <= MICROPY_HW_SPI_MAX) {
        mp_dotmatrix_spi_argcheck(args, &mp_dotmatrix_spi_default_pins[host - 1]);
    } else {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("SPI(%d) doesn't exist"), host);
    }

    // Replace -1 non-pin args with default values
    static const mp_int_t defaults[] = {
        DEFAULT_CASCADE_SIZE, 
        DEFAULT_SCROLL_DELAY, 
        DEFAULT_SPI_HOST, 
        DEFAULT_BAUDRATE, 
        0, 
        0, 
        8, 
        MICROPY_PY_MACHINE_SPI_MSB 
        };

    for (int i = ARG_modules; i <= ARG_firstbit; i++) {
        if (args[i].u_int == -1) {
            args[i].u_int = defaults[i - ARG_modules];
        }
    }

    if (args[ARG_modules].u_int != -1 && args[ARG_modules].u_int != spi->cascade_size) {
        spi->cascade_size = args[ARG_modules].u_int;
        changed = true;
    }

    if (args[ARG_shift_delay].u_int != -1 && args[ARG_shift_delay].u_int != spi->scroll_delay) {
        spi->scroll_delay = args[ARG_shift_delay].u_int;
        changed = true;
    }

    if (args[ARG_baudrate].u_int != -1 && args[ARG_baudrate].u_int != spi->baudrate) {
        spi->baudrate = args[ARG_baudrate].u_int;
        changed = true;
    }

    if (args[ARG_host].u_int != -1 && args[ARG_host].u_int != spi->host) {
        spi->host = args[ARG_host].u_int;
        changed = true;
    }

    if (args[ARG_polarity].u_int != -1 && args[ARG_polarity].u_int != spi->polarity) {
        spi->polarity = args[ARG_polarity].u_int;
        changed = true;
    }

    if (args[ARG_phase].u_int != -1 && args[ARG_phase].u_int != spi->phase) {
        spi->phase = args[ARG_phase].u_int;
        changed = true;
    }

    if (args[ARG_bits].u_int != -1 && args[ARG_bits].u_int != spi->bits) {
        spi->bits = args[ARG_bits].u_int;
        changed = true;
    }

    if (args[ARG_firstbit].u_int != -1 && args[ARG_firstbit].u_int != spi->firstbit) {
        spi->firstbit = args[ARG_firstbit].u_int;
        changed = true;
    }

    if (args[ARG_sck].u_int != -2 && args[ARG_sck].u_int != spi->sck) {
        spi->sck = args[ARG_sck].u_int;
        changed = true;
    }

    if (args[ARG_mosi].u_int != -2 && args[ARG_mosi].u_int != spi->mosi) {
        spi->mosi = args[ARG_mosi].u_int;
        changed = true;
    }

    if (args[ARG_miso].u_int != -2 && args[ARG_miso].u_int != spi->miso) {
        spi->miso = args[ARG_miso].u_int;
        changed = true;
    }

    if (args[ARG_cs].u_int != -1 && args[ARG_cs].u_int != spi->cs) {
        spi->cs = args[ARG_cs].u_int;
        changed = true;
    }

   if (changed) {
        if (spi->state == MAX7219_SPI_STATE_INIT) {
            spi->state = MAX7219_SPI_STATE_DEINIT;
            spi_deinit(&old_dev);
        }
    } else {
        return MP_OBJ_FROM_PTR(self);
    }

    if (1 <= host && host <= MICROPY_HW_SPI_MAX) {
        mp_printf(&mp_plat_print, "SPI host: %d\n", host);
    } else {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("SPI(%d) doesn't exist"), host);
    }

    spi_bus_config_t buscfg = {
        .miso_io_num = spi->miso,
        .mosi_io_num = spi->mosi,
        .sclk_io_num = spi->sck,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = spi->baudrate,
        .mode = spi->phase | (spi->polarity << 1),
        .spics_io_num = spi->cs, // CS pin
        .queue_size = 2,
        .flags = spi->firstbit == MICROPY_PY_MACHINE_SPI_LSB ? SPI_DEVICE_TXBIT_LSBFIRST | SPI_DEVICE_RXBIT_LSBFIRST : 0,
        .pre_cb = NULL
    };

    // Initialize the SPI bus

    // Select DMA channel based on the hardware SPI host
    int dma_chan = 0;
    #if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3 || CONFIG_IDF_TARGET_ESP32C3
    dma_chan = SPI_DMA_CH_AUTO;
    #else
    if (spi->host == SPI2_HOST) {
        dma_chan = 1;
    } else {
        dma_chan = 2;
    }
    #endif

    mp_printf(&mp_plat_print, "Matrix8x8(modules=%u, delay=%u, spi=%u, baudrate=%u, polarity=%u, phase=%u, bits=%u, firstbit=%u, sck=%d, mosi=%d, miso=%d, cs=%d)\n",
    spi->cascade_size, spi->scroll_delay,
    spi->host, spi->baudrate, spi->polarity,
    spi->phase, spi->bits, spi->firstbit,
    spi->sck, spi->mosi, spi->miso, spi->cs);

    ret = spi_bus_initialize(spi->host, &buscfg, dma_chan);
    switch (ret) {
        case ESP_ERR_INVALID_ARG:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("invalid configuration"));
            return MP_OBJ_FROM_PTR(self);

        case ESP_ERR_INVALID_STATE:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("SPI host already in use"));
            return MP_OBJ_FROM_PTR(self);
    }

    ret = spi_bus_add_device(spi->host, &devcfg, &spi->spi_dev);
    switch (ret) {
        case ESP_ERR_INVALID_ARG:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("invalid configuration"));
            spi_bus_free(spi->host);
            return MP_OBJ_FROM_PTR(self);

        case ESP_ERR_NO_MEM:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("out of memory"));
            spi_bus_free(spi->host);
            return MP_OBJ_FROM_PTR(self);

        case ESP_ERR_NOT_FOUND:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("no free slots"));
            spi_bus_free(spi->host);
            return MP_OBJ_FROM_PTR(self);
    }
    spi->state = MAX7219_SPI_STATE_INIT;
    init_display(spi);

    return MP_OBJ_FROM_PTR(self);
}

/**
 * @brief Python write function for writing to the display
 * @param self: instance
 * @param text: Text to write to the display
 * @param centered: Center text if true
 *
 * @returns None 
*/
static mp_obj_t mp_dotmatrix_write(size_t n_args, const mp_obj_t *args) {
    dotmatrix_obj_t *self = MP_OBJ_TO_PTR(args[0]); 
    const char *text = NULL;
    bool centered = false;

    if (n_args > 1) {
        // Convert the MicroPython string to a C string
        text = mp_obj_str_get_str(args[1]);
    }
    // Check if the centered argument was provided
    if (n_args > 2) {
        centered = mp_obj_is_true(args[1]);
    }

    matrixWrite(&(self->dev), text, centered);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(dotmatrix_write_obj, 2, 3, mp_dotmatrix_write);

/**
 * @brief   Write a scrolling text to the display. Needs scroll() 
 *          to actually shift text into display
 * @param text: Text to write to the display
 * 
*/
static mp_obj_t mp_dotmatrix_marquee(mp_obj_t self_in, mp_obj_t text_obj) {
    dotmatrix_obj_t *self = MP_OBJ_TO_PTR(self_in);
    const char *text = mp_obj_str_get_str(text_obj);
    marquee(&(self->dev), text);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(dotmatrix_marquee_obj, mp_dotmatrix_marquee);

/**
 * @brief Shift the text defined through marquee() to the left one pixel position. 
 * 
 * @param self: The instance
 * @param loop: Boolean. Will start scrolling from the right if set True, Default: False
 * @param scroll_delay: Int: Delay in milliseconds between shifting pixels left. Default 
 *                      is unchanged or 30
 * 
 * @returns True if all text was scrolled through, False otherwise
 * 
*/
static mp_obj_t mp_dotmatrix_scroll(size_t n_args, const mp_obj_t *args) {
    dotmatrix_obj_t *self = MP_OBJ_TO_PTR(args[0]); // First argument is always self
    bool wrap = false; // Default value

    if (n_args > 1) {
        wrap = mp_obj_is_true(args[1]);
    }
    if (n_args > 2) {
        self->dev.scroll_delay = mp_obj_get_int(args[2]);
    }
    bool done = scroll(&(self->dev), wrap);
    return mp_obj_new_bool(done);
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(dotmatrix_scroll_obj, 1, 3, mp_dotmatrix_scroll);

/**
 * @brief Clears the display
 * 
 * @param self: The Matrix8x8 object
 * 
*/
static mp_obj_t mp_dotmatrix_clear(mp_obj_t self_in) {
    dotmatrix_obj_t *self = MP_OBJ_TO_PTR(self_in);

    clear(&(self->dev));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(dotmatrix_clear_obj, mp_dotmatrix_clear);

/**
 * @brief Initialise the Matrix8x8 object
 * 
 * @param self: The Matrix8x8 object
*/
static mp_obj_t mp_dotmatrix_init(mp_obj_t self_in) {
    dotmatrix_obj_t *self = MP_OBJ_TO_PTR(self_in);

    init_display(&(self->dev));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(dotmatrix_init_obj, mp_dotmatrix_init);

/**
 * Python Matrix8x8 object to C function mapping
*/
static const mp_rom_map_elem_t dotmatrix_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&dotmatrix_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_marquee), MP_ROM_PTR(&dotmatrix_marquee_obj) },
    { MP_ROM_QSTR(MP_QSTR_scroll), MP_ROM_PTR(&dotmatrix_scroll_obj) },
    { MP_ROM_QSTR(MP_QSTR_clear), MP_ROM_PTR(&dotmatrix_clear_obj) },
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&dotmatrix_init_obj) },
};
static MP_DEFINE_CONST_DICT(dotmatrix_locals_dict, dotmatrix_locals_dict_table);

/**
 *  This defines the type(Matrix8x8) object.
 * */
MP_DEFINE_CONST_OBJ_TYPE(
    dotmatrix_type_Matrix8x8,
    MP_QSTR_Matrix8x8,
    MP_TYPE_FLAG_NONE,
    make_new, dotmatrix_make_new,
    locals_dict, &dotmatrix_locals_dict
    );

/**
 * DotMatrix module define including the Matrix8x8 Class
*/
static const mp_rom_map_elem_t dotmatrix_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_DotMatrix) },
    { MP_ROM_QSTR(MP_QSTR_Matrix8x8),    MP_ROM_PTR(&dotmatrix_type_Matrix8x8) },
};
static MP_DEFINE_CONST_DICT(dotmatrix_globals, dotmatrix_globals_table);

// Define module object.
const mp_obj_module_t dotmatrix_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&dotmatrix_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_DotMatrix, dotmatrix_cmodule);

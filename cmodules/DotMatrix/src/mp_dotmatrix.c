

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "py/runtime.h"
#include "py/stream.h"
#include "py/mphal.h"
#include "extmod/modmachine.h"

#include "driver/spi_master.h"
#include "soc/gpio_sig_map.h"
#include "soc/spi_pins.h"
#include "matrix.h"
#include "max7219.h"

#include <driver/spi_master.h>
#if defined(ESP32)

#elif defined(ESP8266)
#include <esp8266/spi.h>
#endif


typedef struct _mp_dotmatrix_spi_default_pins_t {
    union {
        int8_t array[3];
        struct {
            // Must be in enum's ARG_sck, ARG_mosi, ARG_miso, etc. order
            int8_t sck;
            int8_t mosi;
            int8_t miso;
        } pins;
    };
} mp_dotmatrix_spi_default_pins_t;

typedef struct _dotmatrix_obj_t {
    mp_obj_base_t base;
    max7219_t dev;
} dotmatrix_obj_t;

// Default pin mappings for the hardware SPI instances
static const mp_dotmatrix_spi_default_pins_t mp_dotmatrix_spi_default_pins[MICROPY_HW_SPI_MAX] = {
    { .pins = { .sck = MICROPY_HW_SPI1_SCK, .mosi = MICROPY_HW_SPI1_MOSI, .miso = MICROPY_HW_SPI1_MISO }},
    #ifdef MICROPY_HW_SPI2_SCK
    { .pins = { .sck = MICROPY_HW_SPI2_SCK, .mosi = MICROPY_HW_SPI2_MOSI, .miso = MICROPY_HW_SPI2_MISO }},
    #endif
};

// Common arguments for init() and make new
enum { ARG_modules, ARG_shift_delay, ARG_id, ARG_baudrate, ARG_polarity, ARG_phase, ARG_bits, ARG_firstbit, ARG_sck, ARG_mosi, ARG_miso };
static const mp_arg_t spi_allowed_args[] = {
    { MP_QSTR_modules,  MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = -1} },
    { MP_QSTR_shift_delay,    MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = -1} },
    { MP_QSTR_id,       MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = -1} },
    { MP_QSTR_baudrate, MP_ARG_INT, {.u_int = -1} },
    { MP_QSTR_polarity, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
    { MP_QSTR_phase,    MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
    { MP_QSTR_bits,     MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
    { MP_QSTR_firstbit, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
    { MP_QSTR_sck,      MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
    { MP_QSTR_mosi,     MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
    { MP_QSTR_miso,     MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
};

// Static objects mapping to SPI2 (and SPI3 if available) hardware peripherals.
static dotmatrix_obj_t dotmatrix_obj;


static void mp_dotmatrix_spi_deinit_internal(max7219_t *spi_dev) {
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

    int8_t pins[3] = {spi_dev->miso, spi_dev->mosi, spi_dev->sck};

    for (int i = 0; i < 3; i++) {
        if (pins[i] != -1) {
            esp_rom_gpio_pad_select_gpio(pins[i]);
            esp_rom_gpio_connect_out_signal(pins[i], SIG_GPIO_OUT_IDX, false, false);
            gpio_set_direction(pins[i], GPIO_MODE_INPUT);
        }
    }
}


static void mp_dotmatrix_spi_init_internal(max7219_t *spi_dev, mp_arg_val_t args[]) {

    // if we're not initialized, then we're
    // implicitly 'changed', since this is the init routine
    bool changed = spi_dev->state != MAX7219_SPI_STATE_INIT;

    esp_err_t ret;

    max7219_t old_spi = *spi_dev;

    if (args[ARG_baudrate].u_int != -1) {
        // calculate the actual clock frequency that the SPI peripheral can produce
        uint32_t baudrate = spi_get_actual_clock(APB_CLK_FREQ, args[ARG_baudrate].u_int, 0);
        if (baudrate != spi_dev->baudrate) {
            spi_dev->baudrate = baudrate;
            changed = true;
        }
    }

    if (args[ARG_polarity].u_int != -1 && args[ARG_polarity].u_int != spi_dev->polarity) {
        spi_dev->polarity = args[ARG_polarity].u_int;
        changed = true;
    }

    if (args[ARG_phase].u_int != -1 && args[ARG_phase].u_int != spi_dev->phase) {
        spi_dev->phase = args[ARG_phase].u_int;
        changed = true;
    }

    if (args[ARG_bits].u_int != -1 && args[ARG_bits].u_int != spi_dev->bits) {
        spi_dev->bits = args[ARG_bits].u_int;
        changed = true;
    }

    if (args[ARG_firstbit].u_int != -1 && args[ARG_firstbit].u_int != spi_dev->firstbit) {
        spi_dev->firstbit = args[ARG_firstbit].u_int;
        changed = true;
    }

    if (args[ARG_sck].u_int != -2 && args[ARG_sck].u_int != spi_dev->sck) {
        spi_dev->sck = args[ARG_sck].u_int;
        changed = true;
    }

    if (args[ARG_mosi].u_int != -2 && args[ARG_mosi].u_int != spi_dev->mosi) {
        spi_dev->mosi = args[ARG_mosi].u_int;
        changed = true;
    }

    if (args[ARG_miso].u_int != -2 && args[ARG_miso].u_int != spi_dev->miso) {
        spi_dev->miso = args[ARG_miso].u_int;
        changed = true;
    }

    if (changed) {
        if (spi_dev->state == MAX7219_SPI_STATE_INIT) {
            spi_dev->state = MAX7219_SPI_STATE_DEINIT;
            mp_dotmatrix_spi_deinit_internal(&old_spi);
        }
    } else {
        return; // no changes
    }

    spi_bus_config_t buscfg = {
        .miso_io_num = spi_dev->miso,
        .mosi_io_num = spi_dev->mosi,
        .sclk_io_num = spi_dev->sck,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = spi_dev->baudrate,
        .mode = spi_dev->phase | (spi_dev->polarity << 1),
        .spics_io_num = -1, // No CS pin
        .queue_size = 2,
        .flags = spi_dev->firstbit == MICROPY_PY_MACHINE_SPI_LSB ? SPI_DEVICE_TXBIT_LSBFIRST | SPI_DEVICE_RXBIT_LSBFIRST : 0,
        .pre_cb = NULL
    };

    // Initialize the SPI bus

    // Select DMA channel based on the hardware SPI host
    int dma_chan = 0;
    #if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3 || CONFIG_IDF_TARGET_ESP32C3
    dma_chan = SPI_DMA_CH_AUTO;
    #else
    if (self->host == SPI2_HOST) {
        dma_chan = 1;
    } else {
        dma_chan = 2;
    }
    #endif

    ret = spi_bus_initialize(spi_dev->host, &buscfg, dma_chan);
    switch (ret) {
        case ESP_ERR_INVALID_ARG:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("invalid configuration"));
            return;

        case ESP_ERR_INVALID_STATE:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("SPI host already in use"));
            return;
    }

    ret = spi_bus_add_device(spi_dev->host, &devcfg, &spi_dev->spi_dev);
    switch (ret) {
        case ESP_ERR_INVALID_ARG:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("invalid configuration"));
            spi_bus_free(spi_dev->host);
            return;

        case ESP_ERR_NO_MEM:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("out of memory"));
            spi_bus_free(spi_dev->host);
            return;

        case ESP_ERR_NOT_FOUND:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("no free slots"));
            spi_bus_free(spi_dev->host);
            return;
    }
    spi_dev->state = MAX7219_SPI_STATE_INIT;
}

static void mp_dotmatrix_spi_argcheck(mp_arg_val_t args[], const mp_dotmatrix_spi_default_pins_t *default_pins) {
// A non-NULL default_pins argument will trigger the "use default" behavior.
    // Replace pin args with default/current values for new vs init call, respectively
    for (int i = ARG_sck; i <= ARG_miso; i++) {
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
 * @param blocks: number of 8x8 dot matrix modules cascaded. Default: 8
 * @param scroll_delay: Delay in milliseconds between shifting pixels left. Default is 30
 * @param spi_device: 0 or 1. SPI device to use. Default: SPI2_HOST (=1)
 * @param clock_speed: SPI clocking speed in Hz. Default 1000000 (1 MHz)
 * @param chip_select_pin: CS pin number. Default 5
 * 
 * @returns Matrix8x8 Object initialized
*/
static mp_obj_t dotmatrix_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {

    mp_arg_val_t args[MP_ARRAY_SIZE(spi_allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(spi_allowed_args), spi_allowed_args, args);

    const mp_int_t spi_id = args[ARG_id].u_int;
    if (1 <= spi_id && spi_id <= MICROPY_HW_SPI_MAX) {
        mp_dotmatrix_spi_argcheck(args, &mp_dotmatrix_spi_default_pins[spi_id - 1]);
    } else {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("SPI(%d) doesn't exist"), spi_id);
    }
    // Replace -1 non-pin args with default values
    static const mp_int_t defaults[] = { 500000, 0, 0, 8, MICROPY_PY_MACHINE_SPI_MSB };
    for (int i = ARG_baudrate; i <= ARG_firstbit; i++) {
        if (args[i].u_int == -1) {
            args[i].u_int = defaults[i - ARG_baudrate];
        }
    }

    dotmatrix_obj_t *self = &dotmatrix_obj;
    self->dev->host = spi_id;

    self->base.type = &machine_spi_type;

    mp_dotmatrix_spi_init_internal(self, args);
    init_display(&(self->dev));

    // The make_new function always returns self.
    return MP_OBJ_FROM_PTR(self);
}

// static mp_obj_t dotmatrix_make_new1(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
//     dotmatrix_obj_t *self = mp_obj_malloc(dotmatrix_obj_t, type);

//     uint32_t cascade_size = DEFAULT_CASCADE_SIZE;
//     uint16_t scroll_delay = DEFAULT_SCROLL_DELAY;
//     uint32_t host_device = SPI1_HOST;

//     if (n_args > 0) {
//         cascade_size = mp_obj_get_int(args[0]);
//         mp_printf(&mp_plat_print, "Display blocks: %d\n", cascade_size);
//     }

//     if (n_args > 1) {
//         scroll_delay = mp_obj_get_int(args[1]);
//         mp_printf(&mp_plat_print, "Scroll delay: %dms\n", scroll_delay);
//     }

//     if (n_args > 2) {
//         host_device = mp_obj_get_int(args[2]);
//         mp_printf(&mp_plat_print, "SPI host: %d\n", host_device);
//     }
//     const mp_int_t spi_id = mp_obj_get_int(args[2]);
//     if (1 <= spi_id && spi_id <= MICROPY_HW_SPI_MAX) {
//         mp_printf(&mp_plat_print, "SPI host: %d\n", host_device);
//     } else {
//         mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("SPI(%d) doesn't exist"), spi_id);
//     }

//     // Configure device
//     self->dev.cascade_size = cascade_size;
//     self->dev.scroll_delay = scroll_delay;
//     self->dev.digits = cascade_size*8;
//     self->dev.mirrored = false;
//     self->dev.text = NULL;
//     self->dev.mrqTmstmp = 0;

//    // Configure SPI bus
//     spi_bus_config_t cfg = {
//        .mosi_io_num = DEFAULT_PIN_NUM_MOSI,
//        .miso_io_num = -1,
//        .sclk_io_num = DEFAULT_PIN_NUM_CLK,
//        .quadwp_io_num = -1,
//        .quadhd_io_num = -1,
//        .max_transfer_sz = 0,
//        .flags = 0
//     };
//     spi_bus_initialize(host_device, &cfg, 1);

//     uint32_t clock_speed_hz = MAX7219_MAX_CLOCK_SPEED_HZ;
//     if (n_args > 3) {
//         clock_speed_hz = mp_obj_get_int(args[3]);
//         mp_printf(&mp_plat_print, "Clock speed: %d\n", clock_speed_hz);
//     }

//     mp_uint_t cs_pin = DEFAULT_PIN_CS;
//     if (n_args > 4) {
//         cs_pin = mp_obj_get_int(args[4]);
//         mp_printf(&mp_plat_print, "Chip select: %d\n", cs_pin);
//     }

//     init_descriptor(&(self->dev), host_device, clock_speed_hz, cs_pin);
//     init_display(&(self->dev));

//     // The make_new function always returns self.
//     return MP_OBJ_FROM_PTR(self);
// }

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

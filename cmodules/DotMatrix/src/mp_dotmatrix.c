#include "py/runtime.h"
#include "py/binary.h"
#include "py/objstr.h"
#include "py/obj.h"
#include "matrix.h"
#include "max7219.h"

#if defined(ESP32)
#include <driver/spi_master.h>
#elif defined(ESP8266)
#include <esp8266/spi.h>
#endif

typedef struct _max7219_obj_t {
    mp_obj_base_t base;
    max7219_t dev;
} max7219_obj_t;


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
STATIC mp_obj_t dotmatrix_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    max7219_obj_t *self = mp_obj_malloc(max7219_obj_t, type);

    uint32_t cascade_size = DEFAULT_CASCADE_SIZE;
    uint16_t scroll_delay = DEFAULT_SCROLL_DELAY;
    uint32_t host_device = SPI1_HOST;

    if (n_args > 0) {
        cascade_size = mp_obj_get_int(args[0]);
        mp_printf(&mp_plat_print, "Display blocks: %d\n", cascade_size);
    }

    if (n_args > 1) {
        scroll_delay = mp_obj_get_int(args[1]);
        mp_printf(&mp_plat_print, "Scroll delay: %dms\n", scroll_delay);
    }

    if (n_args > 2) {
        host_device = mp_obj_get_int(args[2]);
        mp_printf(&mp_plat_print, "SPI host: %d\n", host_device);
    }

    // Configure device
    self->dev.cascade_size = cascade_size;
    self->dev.scroll_delay = scroll_delay;
    self->dev.digits = cascade_size*8;
    self->dev.mirrored = false;
    self->dev.text = NULL;
    self->dev.mrqTmstmp = 0;

   // Configure SPI bus
    spi_bus_config_t cfg = {
       .mosi_io_num = DEFAULT_PIN_NUM_MOSI,
       .miso_io_num = -1,
       .sclk_io_num = DEFAULT_PIN_NUM_CLK,
       .quadwp_io_num = -1,
       .quadhd_io_num = -1,
       .max_transfer_sz = 0,
       .flags = 0
    };
    spi_bus_initialize(host_device, &cfg, 1);

    uint32_t clock_speed_hz = MAX7219_MAX_CLOCK_SPEED_HZ;
    if (n_args > 3) {
        clock_speed_hz = mp_obj_get_int(args[3]);
        mp_printf(&mp_plat_print, "Clock speed: %d\n", clock_speed_hz);
    }

    mp_uint_t cs_pin = DEFAULT_PIN_CS;
    if (n_args > 4) {
        cs_pin = mp_obj_get_int(args[4]);
        mp_printf(&mp_plat_print, "Chip select: %d\n", cs_pin);
    }

    init_descriptor(&(self->dev), host_device, clock_speed_hz, cs_pin);
    init_display(&(self->dev));

    // The make_new function always returns self.
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
STATIC mp_obj_t mp_dotmatrix_write(size_t n_args, const mp_obj_t *args) {
    max7219_obj_t *self = MP_OBJ_TO_PTR(args[0]); 
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
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(dotmatrix_write_obj, 2, 3, mp_dotmatrix_write);

/**
 * @brief   Write a scrolling text to the display. Needs scroll() 
 *          to actually shift text into display
 * @param text: Text to write to the display
 * 
*/
STATIC mp_obj_t mp_dotmatrix_marquee(mp_obj_t self_in, mp_obj_t text_obj) {
    max7219_obj_t *self = MP_OBJ_TO_PTR(self_in);
    const char *text = mp_obj_str_get_str(text_obj);
    marquee(&(self->dev), text);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(dotmatrix_marquee_obj, mp_dotmatrix_marquee);

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
STATIC mp_obj_t mp_dotmatrix_scroll(size_t n_args, const mp_obj_t *args) {
    max7219_obj_t *self = MP_OBJ_TO_PTR(args[0]); // First argument is always self
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
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(dotmatrix_scroll_obj, 1, 3, mp_dotmatrix_scroll);

/**
 * @brief Clears the display
 * 
 * @param self: The Matrix8x8 object
 * 
*/
STATIC mp_obj_t mp_dotmatrix_clear(mp_obj_t self_in) {
    max7219_obj_t *self = MP_OBJ_TO_PTR(self_in);

    clear(&(self->dev));
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(dotmatrix_clear_obj, mp_dotmatrix_clear);

/**
 * @brief Initialise the Matrix8x8 object
 * 
 * @param self: The Matrix8x8 object
*/
STATIC mp_obj_t mp_dotmatrix_init(mp_obj_t self_in) {
    max7219_obj_t *self = MP_OBJ_TO_PTR(self_in);

    init_display(&(self->dev));
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(dotmatrix_init_obj, mp_dotmatrix_init);

/**
 * Python Matrix8x8 object to C function mapping
*/
STATIC const mp_rom_map_elem_t dotmatrix_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&dotmatrix_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_marquee), MP_ROM_PTR(&dotmatrix_marquee_obj) },
    { MP_ROM_QSTR(MP_QSTR_scroll), MP_ROM_PTR(&dotmatrix_scroll_obj) },
    { MP_ROM_QSTR(MP_QSTR_clear), MP_ROM_PTR(&dotmatrix_clear_obj) },
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&dotmatrix_init_obj) },
};
STATIC MP_DEFINE_CONST_DICT(dotmatrix_locals_dict, dotmatrix_locals_dict_table);

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
STATIC const mp_rom_map_elem_t dotmatrix_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_DotMatrix) },
    { MP_ROM_QSTR(MP_QSTR_Matrix8x8),    MP_ROM_PTR(&dotmatrix_type_Matrix8x8) },
};
STATIC MP_DEFINE_CONST_DICT(dotmatrix_globals, dotmatrix_globals_table);

// Define module object.
const mp_obj_module_t dotmatrix_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&dotmatrix_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_DotMatrix, dotmatrix_cmodule);

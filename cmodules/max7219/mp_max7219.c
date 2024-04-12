// Include MicroPython API.
#include "py/runtime.h"
#include "py/binary.h"
#include "py/objstr.h"
#include "py/obj.h"
#include "max7219.h"
#include "matrix.h"

typedef struct _max7219_obj_t {
    mp_obj_base_t base;
    max7219_t dev;
} max7219_obj_t;

/**
 * @brief Initialize device
 *
 * @param host SPI host
 * @param clock_speed_hz SPI clock speed, Hz
 * @param cs_pin CS GPIO number
 */
STATIC mp_obj_t max7219_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    max7219_obj_t *self = mp_obj_malloc(max7219_obj_t, type);

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
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &cfg, 1));

    // Configure device
    self->dev.cascade_size = 4; //DEFAULT_CASCADE_SIZE;
    self->dev.scroll_delay = DEFAULT_SCROLL_DELAY;
    self->dev.digits = 4*8;
    self->dev.mirrored = false;
    self->dev.text = NULL;
    self->dev.mrqTmstmp = 0;

    uint32_t clock_speed_hz = MAX7219_MAX_CLOCK_SPEED_HZ;
    if (n_args > 1) {
        clock_speed_hz = mp_obj_get_int(args[1]);
    }

    mp_uint_t cs_pin = DEFAULT_PIN_CS;
    if (n_args > 2) {
        cs_pin = mp_obj_get_int(args[2]);
    }

    max7219_init_desc(&(self->dev), SPI2_HOST, clock_speed_hz, cs_pin);
    max7219_init(&(self->dev));

    // The make_new function always returns self.
    return MP_OBJ_FROM_PTR(self);
}

STATIC mp_obj_t mp_max7219_test(mp_obj_t self_in) {
    max7219_obj_t *self = MP_OBJ_TO_PTR(self_in);
    marquee(&(self->dev), "I Åsane har vi både færøymål, låglønnsnæring og skjærgårdsøl!");
    // bool done = false;
    // while (!done) {
    //     done = scroll(&(self->dev), false);
    // }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(max7219_test_obj, mp_max7219_test);

STATIC mp_obj_t mp_max7219_free(mp_obj_t self_in) {
    max7219_obj_t *self = MP_OBJ_TO_PTR(self_in);
    max7219_free_desc(&(self->dev));
    mp_printf(&mp_plat_print, "max7219 free called\n");
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(max7219_free_obj, mp_max7219_free);

STATIC mp_obj_t mp_max7219_clear(mp_obj_t self_in) {
    max7219_obj_t *self = MP_OBJ_TO_PTR(self_in);

    mp_printf(&mp_plat_print, "max7219 clear called\n");

    max7219_clear(&(self->dev));
    // Get the elapsed time and return it as a MicroPython integer.
    // mp_uint_t elapsed = mp_hal_ticks_ms() - self->start_clear;
    // return mp_obj_new_int_from_uint(elapsed);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(max7219_clear_obj, mp_max7219_clear);

STATIC mp_obj_t mp_max7219_init(mp_obj_t self_in) {
    max7219_obj_t *self = MP_OBJ_TO_PTR(self_in);

    mp_printf(&mp_plat_print, "max7219 init called\n");

    max7219_init(&(self->dev));
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(max7219_init_obj, mp_max7219_init);

STATIC mp_obj_t mp_max7219_decodeMode(mp_obj_t self_in, mp_obj_t bcd) {
    max7219_obj_t *self = MP_OBJ_TO_PTR(self_in);
    max7219_set_decode_mode(&(self->dev), mp_obj_get_int(bcd));
    mp_printf(&mp_plat_print, "max7219 max7219_set_decode_mode called\n");
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(max7219_decodeMode_obj, mp_max7219_decodeMode);

STATIC mp_obj_t mp_max7219_brightness(mp_obj_t self_in, mp_obj_t brightness) {
    max7219_obj_t *self = MP_OBJ_TO_PTR(self_in);
    max7219_set_brightness(&(self->dev), mp_obj_get_int(brightness));
    mp_printf(&mp_plat_print, "max7219 max7219_set_brightness called\n");
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(max7219_brightness_obj, mp_max7219_brightness);

STATIC mp_obj_t mp_max7219_shutDown(mp_obj_t self_in, mp_obj_t shutdown) {
    max7219_obj_t *self = MP_OBJ_TO_PTR(self_in);
    max7219_set_shutdown_mode(&(self->dev), mp_obj_get_int(shutdown));
    mp_printf(&mp_plat_print, "max7219 max7219_set_shutdown_mode called\n");
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(max7219_shutDown_obj, mp_max7219_shutDown);

STATIC mp_obj_t mp_max7219_setDigit(mp_obj_t self_in, mp_obj_t digit, mp_obj_t value) {
    max7219_obj_t *self = MP_OBJ_TO_PTR(self_in);
    max7219_set_digit(&(self->dev), mp_obj_get_int(digit), mp_obj_get_int(value));
    mp_printf(&mp_plat_print, "max7219 draw called\n");
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(max7219_setDigit_obj, mp_max7219_setDigit);

// esp_err_t max7219_draw_image_8x8(max7219_t *dev, uint8_t pos, const void *image);
STATIC mp_obj_t mp_max7219_draw8x8(mp_obj_t self_in, mp_obj_t pos, mp_obj_t image) {
    max7219_obj_t *self = MP_OBJ_TO_PTR(self_in);
    void * img = MP_OBJ_TO_PTR(image);
    max7219_draw_image_8x8(&(self->dev), mp_obj_get_int(pos), img);
    mp_printf(&mp_plat_print, "max7219 draw called\n");
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(max7219_draw8x8_obj, mp_max7219_draw8x8);

STATIC mp_obj_t mp_max7219_marquee(mp_obj_t self_in, mp_obj_t text_obj) {
    max7219_obj_t *self = MP_OBJ_TO_PTR(self_in);
    const char *text = mp_obj_str_get_str(text_obj);
    marquee(&(self->dev), text);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(max7219_marquee_obj, mp_max7219_marquee);

STATIC mp_obj_t mp_max7219_scroll(size_t n_args, const mp_obj_t *args) {
    max7219_obj_t *self = MP_OBJ_TO_PTR(args[0]); // First argument is always self
    bool wrap = false; // Default value
    // Check if the wrap argument was provided
    if (n_args > 1) {
        wrap = mp_obj_is_true(args[1]);
    }
    bool done = scroll(&(self->dev), wrap);
    return mp_obj_new_bool(done);
}
// Define a wrapper function for the variable argument version.
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(max7219_scroll_obj, 1, 2, mp_max7219_scroll);



STATIC mp_obj_t mp_max7219_write(mp_obj_t self_in, mp_obj_t text_obj) {
    max7219_obj_t *self = MP_OBJ_TO_PTR(self_in);

    // Convert the MicroPython string to a C string
    const char *text = mp_obj_str_get_str(text_obj);
    matrixWrite(&(self->dev), text);
    mp_printf(&mp_plat_print, "max7219 matrixWrite called: %s\n", text);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(max7219_write_obj, mp_max7219_write);

STATIC const mp_rom_map_elem_t max7219_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_marquee), MP_ROM_PTR(&max7219_marquee_obj) },
    { MP_ROM_QSTR(MP_QSTR_scroll), MP_ROM_PTR(&max7219_scroll_obj) },
    { MP_ROM_QSTR(MP_QSTR_test), MP_ROM_PTR(&max7219_test_obj) },
    { MP_ROM_QSTR(MP_QSTR_free), MP_ROM_PTR(&max7219_free_obj) },
    { MP_ROM_QSTR(MP_QSTR_clear), MP_ROM_PTR(&max7219_clear_obj) },
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&max7219_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&max7219_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_decodeMode), MP_ROM_PTR(&max7219_decodeMode_obj) },
    { MP_ROM_QSTR(MP_QSTR_brightness), MP_ROM_PTR(&max7219_brightness_obj) },
    { MP_ROM_QSTR(MP_QSTR_shutDown), MP_ROM_PTR(&max7219_shutDown_obj) },
    { MP_ROM_QSTR(MP_QSTR_setDigit), MP_ROM_PTR(&max7219_setDigit_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw8x8), MP_ROM_PTR(&max7219_draw8x8_obj) },
};
STATIC MP_DEFINE_CONST_DICT(max7219_locals_dict, max7219_locals_dict_table);

// This defines the type(Matrix8x8) object.
MP_DEFINE_CONST_OBJ_TYPE(
    max7219_type_Matrix8x8,
    MP_QSTR_Matrix8x8,
    MP_TYPE_FLAG_NONE,
    make_new, max7219_make_new,
    locals_dict, &max7219_locals_dict
    );

STATIC const mp_rom_map_elem_t max7219_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_max7219) },
    { MP_ROM_QSTR(MP_QSTR_Matrix8x8),    MP_ROM_PTR(&max7219_type_Matrix8x8) },
};
STATIC MP_DEFINE_CONST_DICT(max7219_globals, max7219_globals_table);

// Define module object.
const mp_obj_module_t max7219_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&max7219_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_max7219, max7219_cmodule);

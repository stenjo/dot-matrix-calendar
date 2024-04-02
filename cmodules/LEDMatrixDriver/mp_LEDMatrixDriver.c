// Include MicroPython API.
#include "py/runtime.h"
#include "mp_LEDMatrixDriver.h"

// typedef struct _LEDMatrixDriver_obj_t {
//     mp_obj_base_t base;
//     LEDMatrixDriver_t descriptor;
// } LEDMatrixDriver_obj_t;

// STATIC mp_obj_t mp_LEDMatrixDriver_clear(mp_obj_t self_in) {
//     LEDMatrixDriver_obj_t *self = MP_OBJ_TO_PTR(self_in);

//     mp_printf(&mp_plat_print, "LEDMatrixDriver clear called\n");

//     LEDMatrixDriver_clear(&(self->descriptor));
//     // Get the elapsed time and return it as a MicroPython integer.
//     // mp_uint_t elapsed = mp_hal_ticks_ms() - self->start_clear;
//     // return mp_obj_new_int_from_uint(elapsed);
//     return mp_const_none;
// }
// STATIC MP_DEFINE_CONST_FUN_OBJ_1(LEDMatrixDriver_clear_obj, mp_LEDMatrixDriver_clear);

// STATIC mp_obj_t mp_LEDMatrixDriver_init(mp_obj_t self_in) {
//     LEDMatrixDriver_obj_t *self = MP_OBJ_TO_PTR(self_in);

//     mp_printf(&mp_plat_print, "LEDMatrixDriver init called\n");

//     LEDMatrixDriver_init(&(self->descriptor));
//     return mp_const_none;
// }
// STATIC MP_DEFINE_CONST_FUN_OBJ_1(LEDMatrixDriver_init_obj, mp_LEDMatrixDriver_init);

// // esp_err_t LEDMatrixDriver_draw_image_8x8(LEDMatrixDriver_t *dev, uint8_t pos, const void *image);
// STATIC mp_obj_t mp_LEDMatrixDriver_draw8x8(mp_obj_t self_in, mp_obj_t pos, mp_obj_t image) {
//     LEDMatrixDriver_obj_t *self = MP_OBJ_TO_PTR(self_in);
//     void * img = MP_OBJ_TO_PTR(image);
//     mp_printf(&mp_plat_print, "LEDMatrixDriver draw called\n");

//     LEDMatrixDriver_draw_image_8x8(&(self->descriptor), mp_obj_get_int(pos), img);
//     return mp_const_none;
// }
// STATIC MP_DEFINE_CONST_FUN_OBJ_3(LEDMatrixDriver_draw8x8_obj, mp_LEDMatrixDriver_draw8x8);



// // esp_err_t LEDMatrixDriver_init_desc(LEDMatrixDriver_t *dev, spi_host_device_t host, uint32_t clock_speed_hz, gpio_num_t cs_pin);
// STATIC mp_obj_t LEDMatrixDriver_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
//     // Allocates the new object and sets the type.
//     LEDMatrixDriver_obj_t *self = mp_obj_malloc(LEDMatrixDriver_obj_t, type);

//    // Configure SPI bus
//     spi_bus_config_t cfg = {
//        .mosi_io_num = 19,
//        .miso_io_num = -1,
//        .sclk_io_num = 18,
//        .quadwp_io_num = -1,
//        .quadhd_io_num = -1,
//        .max_transfer_sz = 0,
//        .flags = 0
//     };
//     ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &cfg, 1));

//     // Configure device
//     self->descriptor.cascade_size = MAX7219_MAX_CASCADE_SIZE;
//     self->descriptor.digits = 0;
//     self->descriptor.mirrored = true;

//     uint32_t clock_speed_hz = MAX7219_MAX_CLOCK_SPEED_HZ;
//     if (n_args > 1) {
//         clock_speed_hz = mp_obj_get_int(args[1]);
//     }

//     mp_uint_t cs_pin = 15;
//     if (n_args > 2) {
//         cs_pin = mp_obj_get_int(args[2]);
//     }

//     LEDMatrixDriver_init_desc(&(self->descriptor), SPI2_HOST, clock_speed_hz, cs_pin);
//     mp_printf(&mp_plat_print, "LEDMatrixDriver package new or init.f\n");

//     self->descriptor.digits = 8;

//     // The make_new function always returns self.
//     return MP_OBJ_FROM_PTR(self);
// }

STATIC MP_DEFINE_CONST_FUN_OBJ_1(marquee_obj, mp_marquee);


STATIC const mp_rom_map_elem_t LEDMatrixDriver_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_clear), MP_ROM_PTR(&marquee_obj) },
    // { MP_ROM_QSTR(MP_QSTR_clear), MP_ROM_PTR(&LEDMatrixDriver_clear_obj) },
    // { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&LEDMatrixDriver_init_obj) },
    // { MP_ROM_QSTR(MP_QSTR_draw8x8), MP_ROM_PTR(&LEDMatrixDriver_draw8x8_obj) },
};
STATIC MP_DEFINE_CONST_DICT(LEDMatrixDriver_locals_dict, LEDMatrixDriver_locals_dict_table);

// This defines the type(Display) object.
MP_DEFINE_CONST_OBJ_TYPE(
    LEDMatrixDriver_type_Display,
    MP_QSTR_Display,
    MP_TYPE_FLAG_NONE,
    make_new, LEDMatrixDriver_make_new,
    locals_dict, &LEDMatrixDriver_locals_dict
    );

STATIC const mp_rom_map_elem_t LEDMatrixDriver_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_LEDMatrixDriver) },
    { MP_ROM_QSTR(MP_QSTR_Display),    MP_ROM_PTR(&LEDMatrixDriver_type_Display) },
};
STATIC MP_DEFINE_CONST_DICT(LEDMatrixDriver_globals, LEDMatrixDriver_globals_table);

// Define module object.
const mp_obj_module_t LEDMatrixDriver_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&LEDMatrixDriver_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_LEDMatrixDriver, LEDMatrixDriver_cmodule);

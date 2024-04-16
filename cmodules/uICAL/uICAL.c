#include "py/runtime.h"
#include "py/mphal.h"
#include <mp_rrule.h>


// This structure represents RRule instance objects.
typedef struct _RRule_obj_t {
    // All objects start with the base.
    mp_obj_base_t base;
    mp_uint_t start_time;
    rrule_p this;
} RRule_obj_t;


STATIC mp_obj_t uical_RRule_time(mp_obj_t self_in) {
    RRule_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_uint_t elapsed = mp_hal_ticks_ms() - self->start_time;
    return mp_obj_new_int_from_uint(elapsed);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(uical_RRule_time_obj, uical_RRule_time);

// STATIC mp_obj_t uical_RRule_next(mp_obj_t self_in) {
//     // RRule_obj_t *self = MP_OBJ_TO_PTR(self_in);
//     // RRule_next(self->this);
//     return mp_const_none;
// }
// MP_DEFINE_CONST_FUN_OBJ_1(uical_RRule_next_obj, uical_RRule_next);
// STATIC MP_DEFINE_CONST_FUN_OBJ_3(RRule_next_obj, RRule_next);

// This represents RRule.__new__ and RRule.__init__, which is called when
// the user instantiates a RRule object.
STATIC mp_obj_t mp_RRule_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 2, 2, true);
    // Allocates the new object and sets the type.
    RRule_obj_t *self = mp_obj_malloc(RRule_obj_t, type);

    // Initializes the time for this RRule instance.
    self->start_time = mp_hal_ticks_ms();

    const char *rule_text = mp_obj_str_get_str(args[0]);
    const char *dt_start = mp_obj_str_get_str(args[1]);
    mp_printf(&mp_plat_print, "RRule_make_new: n_args: %d, n_kw: %d, args[0]: %s, args[1]: %s\n", n_args, n_kw, rule_text, dt_start);

    self->this = mp_RRule_new(rule_text, dt_start);


    // The make_new function always returns self.
    return MP_OBJ_FROM_PTR(self);
}

// This collects all methods and other static class attributes of the RRule.
// The table structure is similar to the module table, as detailed below.
STATIC const mp_rom_map_elem_t RRule_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_time), MP_ROM_PTR(&uical_RRule_time_obj) },
    // { MP_ROM_QSTR(MP_QSTR_next), MP_ROM_PTR(&uical_RRule_next_obj) },
};
STATIC MP_DEFINE_CONST_DICT(RRule_locals_dict, RRule_locals_dict_table);

// This defines the type(RRule) object.
MP_DEFINE_CONST_OBJ_TYPE(
    uICAL_type_RRule,
    MP_QSTR_RRule,
    MP_TYPE_FLAG_NONE,
    make_new, mp_RRule_make_new,
    locals_dict, &RRule_locals_dict
    );

// Define all attributes of the module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
STATIC const mp_rom_map_elem_t uICAL_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_RRule) },
    // { MP_ROM_QSTR(MP_QSTR_add_ints), MP_ROM_PTR(&uICAL_add_ints_obj) },
    // { MP_ROM_QSTR(MP_QSTR_cppfunc), MP_ROM_PTR(&uICAL_cppfunc_obj) },
    { MP_ROM_QSTR(MP_QSTR_RRule),    MP_ROM_PTR(&uICAL_type_RRule) },
};
STATIC MP_DEFINE_CONST_DICT(uICAL_module_globals, uICAL_module_globals_table);

// Define module object.
const mp_obj_module_t uICAL_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&uICAL_module_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_uICAL, uICAL_cmodule);

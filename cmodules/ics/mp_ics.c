#include "py/runtime.h"
#include "py/binary.h"
#include "py/objstr.h"
#include "py/obj.h"
#include <string.h>
#include "src/ics_parser.h"
#include "src/ics_file.h"

typedef struct {
    mp_obj_base_t base;
    ics_t ics;
} ics_obj_t;

mp_obj_t getEventObj(event_t event) {
    mp_obj_t result;

    if (event.summary != NULL && event.dtstart != NULL) {
        // mp_printf(&mp_plat_print, "summary and start not null\n");
        mp_obj_t summary_obj = mp_obj_new_str(event.summary, strlen(event.summary));
        mp_obj_t dtstart_obj = mp_obj_new_str(event.dtstart, strlen(event.dtstart));
        mp_obj_t tuple[2] = {summary_obj, dtstart_obj};
        result = mp_obj_new_tuple(2, tuple);

        // Memory allocated in ics_parse should be freed after creating MicroPython objects
        free(event.summary);
        free(event.dtstart);
    } else {
        result = mp_const_none;
    }

    return result;
}


STATIC mp_obj_t ics_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args)
{
    ics_obj_t * self = mp_obj_malloc(ics_obj_t, type);
    initIcs(&(self->ics));
    return MP_OBJ_FROM_PTR(self);
}

STATIC mp_obj_t parse_ics(mp_obj_t self_in, mp_obj_t ics_str_obj) {
    
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_check_self(mp_obj_is_str_or_bytes(ics_str_obj));
    GET_STR_DATA_LEN(ics_str_obj, ics_str, ics_str_len);
    

    self->ics = parse((const char *)ics_str);
    mp_obj_t result;

    // mp_printf(&mp_plat_print, "event: %p, summary: %p, dtstart: %p\n", event, event.summary, event.dtstart);
    event_t event = getFirstEvent(&self->ics);
    if (event.summary != NULL && event.dtstart != NULL) {
        // mp_printf(&mp_plat_print, "summary and start not null\n");
        mp_obj_t summary_obj = mp_obj_new_str(event.summary, strlen(event.summary));
        mp_obj_t dtstart_obj = mp_obj_new_str(event.dtstart, strlen(event.dtstart));
        mp_obj_t tuple[2] = {summary_obj, dtstart_obj};
        result = mp_obj_new_tuple(2, tuple);

        // Memory allocated in ics_parse should be freed after creating MicroPython objects
        free(event.summary);
        free(event.dtstart);
    } else {
        // If parsing failed, return None
        // mp_printf(&mp_plat_print, "summary or start null: summary: %p dtstart: %p\n", event.summary, event.dtstart);
        result = mp_const_none;
    }
    
    return result;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(parse_ics_obj, parse_ics);

STATIC mp_obj_t mp_parse_ics_file(mp_obj_t self_in, mp_obj_t file_path_obj) {
    mp_check_self(mp_obj_is_str_or_bytes(file_path_obj));
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);
    const char* file_path = mp_obj_str_get_str(file_path_obj);

    self->ics = parseFile(file_path);
    mp_printf(&mp_plat_print, "ics: %p, count: %d, file: %s\n", &(self->ics), self->ics.count, file_path);
    mp_obj_t result;

    event_t event = getFirstEvent(&self->ics);
    mp_printf(&mp_plat_print, "event: %p, summary: %p, dtstart: %p\n", event, event.summary, event.dtstart);
    return getEventObj(event);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(parse_ics_file_obj, mp_parse_ics_file);

STATIC mp_obj_t mp_getFirst(mp_obj_t self_in) {
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);
    event_t event = getFirstEvent(&self->ics);
    return getEventObj(event);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(getFirst_obj, mp_getFirst);

STATIC mp_obj_t mp_getNext(mp_obj_t self_in) {
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);
    event_t event = getNextEvent(&self->ics);
    return getEventObj(event);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(getNext_obj, mp_getNext);

STATIC const mp_rom_map_elem_t ics_parser_locals_dict_table[] = {

    { MP_ROM_QSTR(MP_QSTR_parse), MP_ROM_PTR(&parse_ics_obj) },
    { MP_ROM_QSTR(MP_QSTR_parse_file), MP_ROM_PTR(&parse_ics_file_obj) },
    { MP_ROM_QSTR(MP_QSTR_getFirst), MP_ROM_PTR(&getFirst_obj) },
    { MP_ROM_QSTR(MP_QSTR_getNext), MP_ROM_PTR(&getNext_obj) },
};

STATIC MP_DEFINE_CONST_DICT(ics_parser_locals_dict, ics_parser_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    ics_parser_type_ICS, 
    MP_QSTR_ICS, 
    MP_TYPE_FLAG_NONE,  
    make_new, ics_make_new, 
    locals_dict, &ics_parser_locals_dict
);

STATIC const mp_map_elem_t ics_parser_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ics_parser) },
    { MP_ROM_QSTR(MP_QSTR_ICS),    MP_ROM_PTR(&ics_parser_type_ICS) },
};

STATIC MP_DEFINE_CONST_DICT(ics_parser_module_globals, ics_parser_module_globals_table);

const mp_obj_module_t ics_parser_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&ics_parser_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_ics_parser, ics_parser_user_cmodule);
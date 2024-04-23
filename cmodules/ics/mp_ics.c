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
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);
    
    mp_check_self(mp_obj_is_str_or_bytes(ics_str_obj));
    GET_STR_DATA_LEN(ics_str_obj, ics_str, ics_str_len);
    
    size_t count = parse(&self->ics, (const char *)ics_str);
    mp_obj_t result = mp_obj_new_int(count);
    
    return result;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(parse_ics_obj, parse_ics);

STATIC mp_obj_t mp_parse_ics_file(mp_obj_t self_in, mp_obj_t file_path_obj) {
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    mp_check_self(mp_obj_is_str_or_bytes(file_path_obj));
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);
    const char* file_path = mp_obj_str_get_str(file_path_obj);

    size_t count = parseFile(&self->ics, file_path);

    mp_obj_t result = mp_obj_new_int(count);
    
    return result;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(parse_ics_file_obj, mp_parse_ics_file);

STATIC mp_obj_t mp_getFirst(mp_obj_t self_in) {
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);
    event_t event = getFirstEvent(&self->ics);
    return getEventObj(event);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(getFirst_obj, mp_getFirst);

STATIC mp_obj_t mp_getNext(mp_obj_t self_in) {
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);
    event_t event = getNextEvent(&(self->ics));
    return getEventObj(event);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(getNext_obj, mp_getNext);

STATIC mp_obj_t mp_getCount(mp_obj_t self_in) {
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_obj_t result = mp_obj_new_int(self->ics.count);
    return result;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(getCount_obj, mp_getCount);

STATIC mp_obj_t mp_getCurrent(mp_obj_t self_in) {
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);
    event_t event = getCurrentEvent(&(self->ics));
    return getEventObj(event);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(getCurrent_obj, mp_getCurrent);


STATIC mp_obj_t mp_getEventAt(mp_obj_t self_in, mp_obj_t mp_index_obj) {
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    mp_check_self(mp_obj_is_integer(mp_index_obj));
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);

    size_t index = mp_obj_get_int(mp_index_obj);
    event_t event = getEventAt(&(self->ics), index);

    return getEventObj(event);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(getEventAt_obj, mp_getEventAt);

STATIC mp_obj_t mp_setCurrentEvent(mp_obj_t self_in, mp_obj_t mp_index_obj) {
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    mp_check_self(mp_obj_is_integer(mp_index_obj));
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);

    size_t index = mp_obj_get_int(mp_index_obj);
    size_t result = setCurrentEvent(&(self->ics), index);

    return mp_obj_new_int(result);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(setCurrentEvent_obj, mp_setCurrentEvent);

STATIC mp_obj_t mp_getNextEventInRange(size_t n_args, const mp_obj_t *args) {
    // args[0] is self, args[1] is start date, and args[2] (optional) is the end date.
    mp_check_self(mp_obj_is_type(args[0], &ics_parser_type_ICS));

    ics_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    const char *start_date_str = mp_obj_str_get_str(args[1]);
    const char *end_date_str = (n_args > 2) ? mp_obj_str_get_str(args[2]) : NULL;

    // Call the C function that filters the events.
    event_t event = getNextEventInRange(&(self->ics), start_date_str, end_date_str);

    // Return the event as a MicroPython object.
    return getEventObj(event);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mp_getNextEventInRange_obj, 2, 3, mp_getNextEventInRange);

STATIC mp_obj_t mp_setStartDate(mp_obj_t self_in, mp_obj_t start_date_obj) {
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    mp_check_self(mp_obj_is_str_or_bytes(start_date_obj));
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);
    const char* startDate = mp_obj_str_get_str(start_date_obj);

    size_t result = setStartDate(&(self->ics), startDate);

    return mp_obj_new_int(result);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(setStartDate_obj, mp_setStartDate);


STATIC const mp_rom_map_elem_t ics_parser_locals_dict_table[] = {

    { MP_ROM_QSTR(MP_QSTR_parse), MP_ROM_PTR(&parse_ics_obj) },
    { MP_ROM_QSTR(MP_QSTR_parse_file), MP_ROM_PTR(&parse_ics_file_obj) },
    { MP_ROM_QSTR(MP_QSTR_getFirst), MP_ROM_PTR(&getFirst_obj) },
    { MP_ROM_QSTR(MP_QSTR_getNext), MP_ROM_PTR(&getNext_obj) },
    { MP_ROM_QSTR(MP_QSTR_getCount), MP_ROM_PTR(&getCount_obj) },
    { MP_ROM_QSTR(MP_QSTR_getCurrent), MP_ROM_PTR(&getCurrent_obj) },
    { MP_ROM_QSTR(MP_QSTR_getEventAt), MP_ROM_PTR(&getEventAt_obj) },
    { MP_ROM_QSTR(MP_QSTR_setCurrentEvent), MP_ROM_PTR(&setCurrentEvent_obj) },
    { MP_ROM_QSTR(MP_QSTR_getNextEventInRange), MP_ROM_PTR(&mp_getNextEventInRange_obj) },
    { MP_ROM_QSTR(MP_QSTR_setStartDate), MP_ROM_PTR(&setStartDate_obj) },
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
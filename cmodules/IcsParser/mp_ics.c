#include "py/runtime.h"
#include "py/binary.h"
#include "py/objstr.h"
#include "py/obj.h"
#include <string.h>
#include "src/ics_parser.h"
#include "src/ics_event.h"
#include "src/event.h"
#include "src/ics.h"

typedef struct {
    mp_obj_base_t base;
    ics_t ics;
} ics_obj_t;

static mp_obj_t getEventObj(event_t * event) {
    mp_obj_t result;

    if (event != NULL && event->summary != NULL && event->dtstart != NULL) {
        // mp_printf(&mp_plat_print, "summary and start not null\n");
        // mp_printf(&mp_plat_print, "dtend: %s\n", event.dtend);
        mp_obj_t summary_obj = mp_obj_new_str(event->summary, strlen(event->summary));
        mp_obj_t dtstart_obj = mp_obj_new_str(event->dtstart, strlen(event->dtstart));
        mp_obj_t dtend_obj = mp_const_none;
        if (event->dtend != NULL) {
            dtend_obj = mp_obj_new_str(event->dtend, strlen(event->dtend));
        }
        mp_obj_t tuple[3] = {summary_obj, dtstart_obj, dtend_obj};
        result = mp_obj_new_tuple(3, tuple);
    } else {
        result = mp_const_none;
    }

    return result;
}

static mp_obj_t ics_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args)
{
    ics_obj_t * self = mp_obj_malloc(ics_obj_t, type);
    initIcs(&(self->ics));
    self->ics.startTime = 0;
    self->ics.endTime = 0;

    return MP_OBJ_FROM_PTR(self);
}

static mp_obj_t parse_ics(mp_obj_t self_in, mp_obj_t ics_str_obj) {
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);
    
    mp_check_self(mp_obj_is_str_or_bytes(ics_str_obj));
    GET_STR_DATA_LEN(ics_str_obj, ics_str, ics_str_len);
    
    size_t count = parseIcs(&self->ics, (const char *)ics_str);
    sortEventsByStart(&(self->ics));
    mp_obj_t result = mp_obj_new_int(count);
    
    return result;
}
static MP_DEFINE_CONST_FUN_OBJ_2(parse_ics_obj, parse_ics);

static mp_obj_t mp_parse_ics_file(mp_obj_t self_in, mp_obj_t file_path_obj) {
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    mp_check_self(mp_obj_is_str_or_bytes(file_path_obj));
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);
    const char* file_path = mp_obj_str_get_str(file_path_obj);

    size_t count = parseFile(&self->ics, file_path);

    mp_obj_t result = mp_obj_new_int(count);
    
    return result;
}
static MP_DEFINE_CONST_FUN_OBJ_2(parse_ics_file_obj, mp_parse_ics_file);

static mp_obj_t mp_getFirst(mp_obj_t self_in) {
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);
    event_t * event = getFirstEvent(&(self->ics));
    // mp_printf(&mp_plat_print, "mp_getFirst called: %s\n", event.summary);
    return getEventObj(event);
}
static MP_DEFINE_CONST_FUN_OBJ_1(getFirst_obj, mp_getFirst);

static mp_obj_t mp_getNext(mp_obj_t self_in) {
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);
    event_t * event_p = getNextEvent(&(self->ics));
    if (event_p != NULL) {
        return getEventObj(event_p);
    }
    mp_printf(&mp_plat_print, "mp_getNext called: %p\n", event_p);
    return getEventObj(NULL);
}
static MP_DEFINE_CONST_FUN_OBJ_1(getNext_obj, mp_getNext);

static mp_obj_t mp_getCount(mp_obj_t self_in) {
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_obj_t result = mp_obj_new_int(self->ics.count);
    return result;
}
static MP_DEFINE_CONST_FUN_OBJ_1(getCount_obj, mp_getCount);

static mp_obj_t mp_getCurrent(mp_obj_t self_in) {
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);
    event_t * event = getCurrentEvent(&(self->ics));
    return getEventObj(event);
}
static MP_DEFINE_CONST_FUN_OBJ_1(getCurrent_obj, mp_getCurrent);


static mp_obj_t mp_getEventAt(mp_obj_t self_in, mp_obj_t mp_index_obj) {
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    mp_check_self(mp_obj_is_integer(mp_index_obj));
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);

    size_t index = mp_obj_get_int(mp_index_obj);
    event_t *event = getEventAt(&(self->ics), index);

    return getEventObj(event);
}
static MP_DEFINE_CONST_FUN_OBJ_2(getEventAt_obj, mp_getEventAt);

static mp_obj_t mp_setCurrentEvent(mp_obj_t self_in, mp_obj_t mp_index_obj) {
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    mp_check_self(mp_obj_is_integer(mp_index_obj));
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);

    size_t index = mp_obj_get_int(mp_index_obj);
    size_t result = setCurrentEvent(&(self->ics), index);

    return mp_obj_new_int(result);
}
static MP_DEFINE_CONST_FUN_OBJ_2(setCurrentEvent_obj, mp_setCurrentEvent);

/**
 * @brief Set start date of earliest possible event start to parse
 * @param self_in Pointer to Self
 * @param start_date_obj datetime in the format 'yyyymmddThhmmssZ'
 * 
 * @return timestamp from converted date
*/
static mp_obj_t mp_setStartDate(mp_obj_t self_in, mp_obj_t start_date_obj) {
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);

    mp_check_self(mp_obj_is_str_or_bytes(start_date_obj));

    const char* startDate = mp_obj_str_get_str(start_date_obj);
    // mp_printf(&mp_plat_print, "mp_setStartDate called: %s\n", startDate);

    size_t result = setStartDate(&(self->ics), startDate);

    return mp_obj_new_int(result);
}
static MP_DEFINE_CONST_FUN_OBJ_2(setStartDate_obj, mp_setStartDate);


static mp_obj_t mp_setEndDate(mp_obj_t self_in, mp_obj_t end_date_obj) {
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);

    mp_check_self(mp_obj_is_str_or_bytes(end_date_obj));
    // GET_STR_DATA_LEN(end_date_obj, end_date, end_date_len);
    const char* endDate = mp_obj_str_get_str(end_date_obj);

    size_t result = setEndDate(&(self->ics), endDate);

    return mp_obj_new_int(result);
}
static MP_DEFINE_CONST_FUN_OBJ_2(setEndDate_obj, mp_setEndDate);

static mp_obj_t mp_reset(mp_obj_t self_in) {
    mp_check_self(mp_obj_is_type(self_in, &ics_parser_type_ICS)); // Modify the type check accordingly
    ics_obj_t *self = MP_OBJ_TO_PTR(self_in);
    freeIcs(&(self->ics));
    initIcs(&(self->ics));
    initIcsDates(&(self->ics));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(reset_obj, mp_reset);


static const mp_rom_map_elem_t ics_parser_locals_dict_table[] = {

    { MP_ROM_QSTR(MP_QSTR_parseIcs), MP_ROM_PTR(&parse_ics_obj) },
    { MP_ROM_QSTR(MP_QSTR_parse_ics_file), MP_ROM_PTR(&parse_ics_file_obj) },
    { MP_ROM_QSTR(MP_QSTR_getFirst), MP_ROM_PTR(&getFirst_obj) },
    { MP_ROM_QSTR(MP_QSTR_getNext), MP_ROM_PTR(&getNext_obj) },
    { MP_ROM_QSTR(MP_QSTR_getCount), MP_ROM_PTR(&getCount_obj) },
    { MP_ROM_QSTR(MP_QSTR_getCurrent), MP_ROM_PTR(&getCurrent_obj) },
    { MP_ROM_QSTR(MP_QSTR_getEventAt), MP_ROM_PTR(&getEventAt_obj) },
    { MP_ROM_QSTR(MP_QSTR_setCurrentEvent), MP_ROM_PTR(&setCurrentEvent_obj) },
    { MP_ROM_QSTR(MP_QSTR_setStartDate), MP_ROM_PTR(&setStartDate_obj) },
    { MP_ROM_QSTR(MP_QSTR_setEndDate), MP_ROM_PTR(&setEndDate_obj) },
    { MP_ROM_QSTR(MP_QSTR_reset), MP_ROM_PTR(&reset_obj) },
};

static MP_DEFINE_CONST_DICT(ics_parser_locals_dict, ics_parser_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    ics_parser_type_ICS, 
    MP_QSTR_ICS, 
    MP_TYPE_FLAG_NONE,  
    make_new, ics_make_new, 
    locals_dict, &ics_parser_locals_dict
);

static const mp_rom_map_elem_t ics_parser_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ics_parser) },
    { MP_ROM_QSTR(MP_QSTR_ICS),     MP_ROM_PTR(&ics_parser_type_ICS) }
};

static MP_DEFINE_CONST_DICT(ics_parser_module_globals, ics_parser_module_globals_table);

const mp_obj_module_t ics_parser_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&ics_parser_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_ics_parser, ics_parser_user_cmodule);
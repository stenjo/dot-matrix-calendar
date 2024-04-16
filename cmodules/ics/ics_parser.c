#include "py/runtime.h"
#include "py/objstr.h"
#include <string.h>

STATIC mp_obj_t parse_ics(mp_obj_t ics_str_obj) {
    mp_check_self(mp_obj_is_str_or_bytes(ics_str_obj));
    GET_STR_DATA_LEN(ics_str_obj, ics_str, ics_str_len);

    const char *key_vevent_start = "BEGIN:VEVENT";
    const char *key_vevent_end = "END:VEVENT";
    const char *key_summary = "SUMMARY:";
    const char *key_dtstart = "DTSTART:";

    // We will only parse the first VEVENT found
    char *vevent_start = strstr((const char *)ics_str, key_vevent_start);
    char *vevent_end = vevent_start ? strstr(vevent_start, key_vevent_end) : NULL;
    char *summary_start = vevent_start ? strstr(vevent_start, key_summary) : NULL;
    char *dtstart_start = vevent_start ? strstr(vevent_start, key_dtstart) : NULL;

    if (vevent_start && vevent_end && summary_start && dtstart_start) {
        summary_start += strlen(key_summary);
        dtstart_start += strlen(key_dtstart);

        char *summary_end = strchr(summary_start, '\r');
        char *dtstart_end = strchr(dtstart_start, '\r');

        mp_obj_t summary_obj = mp_obj_new_str(summary_start, summary_end - summary_start);
        mp_obj_t dtstart_obj = mp_obj_new_str(dtstart_start, dtstart_end - dtstart_start);

        mp_obj_t tuple[2] = {summary_obj, dtstart_obj};
        return mp_obj_new_tuple(2, tuple);
    }
    
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(parse_ics_obj, parse_ics);

STATIC const mp_map_elem_t ics_parser_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_ics_parser) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_parse), (mp_obj_t)&parse_ics_obj },
};

STATIC MP_DEFINE_CONST_DICT(ics_parser_module_globals, ics_parser_module_globals_table);

const mp_obj_module_t ics_parser_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&ics_parser_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_ics_parser, ics_parser_user_cmodule);
#ifndef mp_rrule_h
#define mp_rrule_h

#ifdef __cplusplus
extern "C" {
#endif

// Include MicroPython API.
#include "py/runtime.h"

    typedef void* rrule_p;

    rrule_p RRule_new(const char * rrule, const char * dtstart);
    void RRule_del(rrule_p p);

    bool RRule_next(rrule_p p);
    void RRule_now(rrule_p p);

// Declare the function we'll make available in Python as cppexample.cppfunc().
extern mp_obj_t cppfunc(mp_obj_t a_obj, mp_obj_t b_obj);

#ifdef __cplusplus
}
#endif

#endif


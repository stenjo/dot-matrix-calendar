#include "includes/cppstl.h"
#include "includes/types.h"
#include "includes/util.h"
#include "includes/datetime.h"
#include "includes/rruleiter.h"
extern "C" {
// #include <uICAL.h>
#include <py/objstr.h>
#include <mp_rrule.h>

// Here we implement the function using C++ code, but since it's
// declaration has to be compatible with C everything goes in extern "C" scope.
mp_obj_t cppfunc(mp_obj_t a_obj, mp_obj_t b_obj) {
    // The following no-ops are just here to verify the static assertions used in
    // the public API all compile with C++.
    MP_STATIC_ASSERT_STR_ARRAY_COMPATIBLE;
    if (mp_obj_is_type(a_obj, &mp_type_BaseException)) {
    }

    // Prove we have (at least) C++11 features.
    const auto a = mp_obj_get_int(a_obj);
    const auto b = mp_obj_get_int(b_obj);
    const auto sum = [&]() {
        return mp_obj_new_int(a + b);
    } ();
    // Prove we're being scanned for QSTRs.
    mp_obj_t tup[] = {sum, MP_ROM_QSTR(MP_QSTR_hellocpp)};
    return mp_obj_new_tuple(2, tup);
}

using rrule_t = uICAL::RRuleIter *;

rrule_p RRule_new(const char * rrule, const char * dtstart) {
    printf("RRule_new called with: %s, start: %s\n", rrule, dtstart);
    return (rrule_p) 0x10000;
}

bool RRule_next(rrule_p p) {
    // auto rule = static_cast<uICAL::RRuleIter*>(p);
    // return rule->next();
    printf("RRule_next called with: %p\n", p);
    return true;
}

}

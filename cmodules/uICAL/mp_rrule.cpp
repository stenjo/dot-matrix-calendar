#include <py/objstr.h>
// #include "includes/cppstl.h"
// #include "includes/types.h"
// #include "includes/util.h"
// #include "includes/datetime.h"
// #include "includes/rruleiter.h"
#include <includes/rrule.h>
#include <mp_rrule.h>

extern "C" {
    // using rrule_t = uICAL::RRuleIter *;

    rrule_p mp_RRule_new(const char * rrule, const char * dtstart) 
    {
        printf("RRule_new called with: %s, start: %s\n", rrule, dtstart);
        // auto rule = new uICAL::RRule(rrule, uICAL::DateTime(dtstart));
        // auto rule = new uICAL::RRule(rrule);
        // return static_cast<rrule_p>(rule);
        return static_cast<rrule_p>(nullptr);
    }

    // bool mp_RRule_next(rrule_p p) {
    //     // auto rule = static_cast<uICAL::RRuleIter*>(p);
    //     // return rule->next();
    //     printf("RRule_next called with: %p\n", p);
    //     return true;
    // }

}

#ifndef mp_rrule_h
#define mp_rrule_h

#ifdef __cplusplus
extern "C" {
#endif

    typedef void* rrule_p;
    rrule_p mp_RRule_new(const char * rrule, const char * dtstart);
    // void mp_RRule_del(rrule_p p);

    // bool mp_RRule_next(rrule_p p);
    // void mp_RRule_now(rrule_p p);

#ifdef __cplusplus
}
#endif

#endif


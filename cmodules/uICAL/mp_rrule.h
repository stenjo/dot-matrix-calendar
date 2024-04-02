#ifndef mp_rrule_h
#define mp_rrule_h

#ifdef __cplusplus
extern "C" {
#endif

    typedef void* rrule_p;

    rrule_p RRule_new(char * rrule, char * dtstart);
    void RRule_del(rrule_p p);

    bool RRule_next(rrule_p p);
    void RRule_now(rrule_p p);

#ifdef __cplusplus
}
#endif

#endif


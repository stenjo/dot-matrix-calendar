#ifdef TEST

#include <string.h>
#include <stdlib.h>
#include "unity.h"

#include "ics_parser.h"
#include "ics_event.h"
#include "ics_utils.h"

const char *ics_repeated_2weekly_timeofday = "BEGIN:VCALENDAR\r\n"
                      "VERSION:2.0\r\n"
                      "BEGIN:VEVENT\r\n"
                      "DTSTART:20240109T211509Z\r\n"
                      "DTEND:20240109T221509Z\r\n"
                      "RRULE:FREQ=WEEKLY;INTERVAL=2\r\n"
                      "DTSTAMP:20240424T062905Z\r\n"
                      "UID:90ocma42249tokq85a3nuljgl0@google.com\r\n"
                      "CREATED:20240101T133719Z\r\n"
                      "LAST-MODIFIED:20240415T073040Z\r\n"
                      "SEQUENCE:0\r\n"
                      "STATUS:CONFIRMED\r\n"
                      "SUMMARY:Matavfall\r\n"
                      "TRANSP:TRANSPARENT\r\n"
                      "BEGIN:VALARM\r\n"
                      "ACTION:AUDIO\r\n"
                      "TRIGGER:-PT15H\r\n"
                      "ACKNOWLEDGED:20240415T073031Z\r\n"
                      "ATTACH;VALUE=URI:Chord\r\n"
                      "UID:604C15F2-C820-43EE-8559-A557C29BFC78\r\n"
                      "X-APPLE-DEFAULT-ALARM:TRUE\r\n"
                      "X-WR-ALARMUID:604C15F2-C820-43EE-8559-A557C29BFC78\r\n"
                      "END:VALARM\r\n"
                      "END:VEVENT\r\n"
                      "END:VCALENDAR";

const char *ics_repeated_4weekly = "BEGIN:VCALENDAR\r\n"
                      "VERSION:2.0\r\n"
                      "BEGIN:VEVENT\r\n"
                      "DTSTART;VALUE=DATE:20240116\r\n"
                      "DTEND;VALUE=DATE:20240117\r\n"
                      "RRULE:FREQ=WEEKLY;INTERVAL=4\r\n"
                      "DTSTAMP:20240424T062905Z\r\n"
                      "UID:bug3i5ig6j8v71399cqi8ascco@google.com\r\n"
                      "CREATED:20240101T133755Z\r\n"
                      "LAST-MODIFIED:20240408T100539Z\r\n"
                      "SEQUENCE:0\r\n"
                      "STATUS:CONFIRMED\r\n"
                      "SUMMARY:Restavfall\r\n"
                      "TRANSP:TRANSPARENT\r\n"
                      "X-APPLE-TRAVEL-ADVISORY-BEHAVIOR:AUTOMATIC\r\n"
                      "BEGIN:VALARM\r\n"
                      "ACTION:AUDIO\r\n"
                      "TRIGGER:-PT15H\r\n"
                      "X-WR-ALARMUID:239590F8-F791-4A22-994D-6FB9571B8038\r\n"
                      "UID:239590F8-F791-4A22-994D-6FB9571B8038V\r\n"
                      "ATTACH;VALUE=URI:Chord\r\n"
                      "X-APPLE-DEFAULT-ALARM:TRUE\r\n"
                      "ACKNOWLEDGED:20240408T100536Z\r\n"
                      "END:VALARM\r\n"
                      "END:VEVENT";

const char *ics_data3 = "BEGIN:VCALENDAR\r\n"
                      "VERSION:2.0\r\n"
                      "BEGIN:VEVENT\r\n"
                      "SUMMARY:Meeting with John\r\n"
                      "DTSTART:20230412T160000Z\r\n"
                      "END:VEVENT\r\n"
                      "BEGIN:VEVENT\r\n"
                      "UID:http://2024.f2cal.com/#GP3_2024_qualifying\r\n"
                      "SUMMARY:F2: Kvalifisering (Emilia Romagna)\r\n"
                      "DTSTAMP:20240405T155803Z\r\n"
                      "DTSTART:20240517T140000Z\r\n"
                      "DTEND:20240517T143000Z\r\n"
                      "SEQUENCE:2024\r\n"
                      "GEO:44.344576;11.713808\r\n"
                      "LOCATION:Imola\r\n"
                      "STATUS:CONFIRMED\r\n"
                      "CATEGORIES:Kvalifisering,F2\r\n"
                      "END:VEVENT\r\n"
                      "BEGIN:VEVENT\r\n"
                      "UID:http://2024.f2cal.com/#GP4_2024_feature\r\n"
                      "SUMMARY:F2: Feature (Monaco)\r\n"
                      "DTSTAMP:20240405T155803Z\r\n"
                      "DTSTART:20240526T084000Z\r\n"
                      "DTEND:20240526T094000Z\r\n"
                      "SEQUENCE:2024\r\n"
                      "GEO:43.7338;7.4215\r\n"
                      "LOCATION:Monte Carlo\r\n"
                      "STATUS:CONFIRMED\r\n"
                      "CATEGORIES:Feature,F2\r\n"
                      "END:VEVENT\r\n"
                      "END:VCALENDAR";

void setUp(void) {
    resetGetEvent();
}

void tearDown(void) {
    // Tear down code if needed
}


void test_parse_with_start_and_end_dates_repeat(void) {
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);

    const char *startDate = "20240716T000000Z";
    const char *endDate = "20240731T235959Z";
    ics.startTime = setStartDate(&ics, startDate);
    ics.endTime = setEndDate(&ics, endDate);

    size_t count = parseIcs(&ics, ics_repeated_4weekly);
    event_t * event;
    while (!atEnd(&ics))  {
        event = getNextEvent(&ics);
        printf("%s %s %s\n", event->dtstart, event->dtend, event->summary);
    }

    TEST_ASSERT_EQUAL(1, count);
    TEST_ASSERT_EQUAL_STRING("Restavfall", ics.events[0]->summary);
    TEST_ASSERT_EQUAL_STRING("20240730", ics.events[0]->dtstart);
    TEST_ASSERT_EQUAL_STRING("20240731", ics.events[0]->dtend);
    TEST_ASSERT_EQUAL_STRING("WEEKLY", ics.events[0]->rrule);
    TEST_ASSERT_EQUAL_STRING("4", ics.events[0]->interval);

    freeIcs(&ics);
}

void test_parse_with_start_and_end_times_repeat(void) {
    ics_t ics;
    initIcs(&ics);
    initIcsDates(&ics);

    const char *startDate = "20240716T000000Z";
    const char *endDate = "20240807T235959Z";
    ics.startTime = setStartDate(&ics, startDate);
    ics.endTime = setEndDate(&ics, endDate);

    size_t count = parseIcs(&ics, ics_repeated_2weekly_timeofday);

    event_t * event;
    while (!atEnd(&ics))  {
        event = getNextEvent(&ics);
        printf("%s %s %s\n", event->dtstart, event->dtend, event->summary);
    }

    TEST_ASSERT_EQUAL(2, count);
    TEST_ASSERT_EQUAL_STRING("Matavfall", ics.events[0]->summary);
    TEST_ASSERT_INT_WITHIN(4000, getTimeStamp("20240723T211509Z"), ics.events[0]->tstart);
    TEST_ASSERT_INT_WITHIN(4000, getTimeStamp("20240723T221509Z"), ics.events[0]->tend);
    TEST_ASSERT_EQUAL_STRING("WEEKLY", ics.events[0]->rrule);
    TEST_ASSERT_EQUAL_STRING("2", ics.events[0]->interval);

    freeIcs(&ics);
}

#endif // TEST

#ifdef TEST

#include "unity.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "../src/ics_parser.h"

const char *ics_data = "BEGIN:VCALENDAR\r\n"
                      "VERSION:2.0\r\n"
                      "BEGIN:VEVENT\r\n"
                      "SUMMARY:Meeting with John\r\n"
                      "DTSTART:20230412T160000Z\r\n"
                      "END:VEVENT\r\n"
                      "END:VCALENDAR";

const char *ics_all_day = "BEGIN:VCALENDAR\r\n"
                      "VERSION:2.0\r\n"
                      "BEGIN:VEVENT\r\n"
                      "SUMMARY:Meeting with John\r\n"
                      "DTSTART:20230412T160000Z\r\n"
                      "END:VEVENT\r\n"
                      "END:VCALENDAR";

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


void setUp(void)
{
}

void tearDown(void)
{
}

// Function to test the parsing of a single event with chunked data
void test_parse_single_event_chunked() {
    ics_t ics;
    initIcs(&ics);

    // First chunk of data (incomplete event)
    char *ics_data_chunk1 = "BEGIN:VEVENT\r\nSUMMARY:Sample Event\r\nDTSTART:20230610T080000Z\r\n";
    size_t chunk1_size = strlen(ics_data_chunk1);
    size_t count = parse(&ics, ics_data_chunk1, chunk1_size);

    // Ensure no events are parsed from the first chunk
    if (count == 0) {
        printf("Test passed: No events parsed from incomplete first chunk\n");
    } else {
        printf("Test failed: Expected 0 events, but parsed %zu events from first chunk\n", count);
    }

    // Second chunk of data (completes the event)
    char *ics_data_chunk2 = "DTEND:20230610T090000Z\r\nEND:VEVENT\r\n";
    size_t chunk2_size = strlen(ics_data_chunk2);
    count = parse(&ics, ics_data_chunk2, chunk2_size);

    // Ensure one event is parsed from the combined chunks
    if (count == 1) {
        printf("Test passed: Single event parsed successfully from combined chunks\n");
    } else {
        printf("Test failed: Expected 1 event, but parsed %zu events from combined chunks\n", count);
    }

    freeIcs(&ics);
}

// Function to test the parsing of multiple events with chunked data
void test_parse_multiple_events_chunked() {
    ics_t ics;
    initIcs(&ics);

    // First chunk of data (first event complete, second event incomplete)
    char *ics_data_chunk1 = "BEGIN:VEVENT\r\nSUMMARY:Event 1\r\nDTSTART:20230610T080000Z\r\nDTEND:20230610T090000Z\r\nEND:VEVENT\r\n"
                            "BEGIN:VEVENT\r\nSUMMARY:Event 2\r\nDTSTART:20230611T080000Z\r\n";
    size_t chunk1_size = strlen(ics_data_chunk1);
    size_t count = parse(&ics, ics_data_chunk1, chunk1_size);

    // Ensure one event is parsed from the first chunk
    if (count == 1) {
        printf("Test passed: One event parsed from first chunk\n");
    } else {
        printf("Test failed: Expected 1 event, but parsed %zu events from first chunk\n", count);
    }

    // Second chunk of data (completes the second event)
    char *ics_data_chunk2 = "DTEND:20230611T090000Z\r\nEND:VEVENT\r\n";
    size_t chunk2_size = strlen(ics_data_chunk2);
    count = parse(&ics, ics_data_chunk2, chunk2_size);

    // Ensure two events are parsed from the combined chunks
    if (count == 2) {
        printf("Test passed: Two events parsed successfully from combined chunks\n");
    } else {
        printf("Test failed: Expected 2 events, but parsed %zu events from combined chunks\n", count);
    }

    freeIcs(&ics);
}

int main() {
    test_parse_single_event_chunked();
    test_parse_multiple_events_chunked();
    return 0;
}


#endif // TEST

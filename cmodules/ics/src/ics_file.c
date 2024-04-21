#include <stdio.h>
#include <stdlib.h>
#include "ics_file.h"
#include "ics_parser.h"

ics_t parseFile(const char *file_path) {
    // Allocate a buffer for the file's contents
    char *buffer = NULL;
    ics_t ics;
    initIcs(&ics);
    long length;
    FILE *f = fopen(file_path, "r");  // Open file for reading

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length + 1);

        if (buffer) {
            fread(buffer, 1, length, f);
            buffer[length] = '\0';  // Null-terminate the buffer
        }
        fclose(f);

        if (buffer) {
            // Parse the ICS data from the buffer
            ics = parse(buffer);
            free(buffer);  // Don't forget to free the allocated buffer
        }
    }

    return ics;
}
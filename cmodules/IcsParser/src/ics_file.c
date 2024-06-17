#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "ics_parser.h"

size_t parseFile(ics_t * ics, const char *file_path) {
    // Allocate a buffer for the file's contents
    char *buffer = NULL;
    // initIcs(ics);
    long length;
    FILE *f = fopen(file_path, "rb");  // Open file for reading

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
            parse(ics, buffer);
            free(buffer);  // Don't forget to free the allocated buffer
        }
     } else {
        // Print an error message if the file could not be opened
        printf("Error opening file '%s': %d\n", file_path, errno);
    }

    return ics->count;
}
#ifndef ICS_FILE_H
#define ICS_FILE_H

#include "ics_parser.h"

// Define function to parse ICS data from a file
ics_t parseFile(ics_t * ics, const char *file_path);

#endif // ICS_FILE_H
#ifndef ICS_FILE_H
#define ICS_FILE_H

#include "ics_parser.h"

/**
 * @brief: Parses a file and loads into the event list of the ics structure
 * 
 * @param ics calendar descriptor
 * @return number of events loaded
*/
size_t parseFile(ics_t * ics, const char *file_path);

size_t loadFileRef(const char *file_path);

#endif // ICS_FILE_H
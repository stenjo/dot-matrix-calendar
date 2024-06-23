# Create an INTERFACE library for our C module.
add_library(usermod_ics INTERFACE)

# Add our source files to the lib
target_sources(usermod_ics INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/ics_parser.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ics_event.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ics_utils.c
    ${CMAKE_CURRENT_LIST_DIR}/mp_ics.c
)

# Add the current directory as an include directory.
target_include_directories(usermod_ics INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_ics)

# Create an INTERFACE library for our C module.
add_library(usermod_uICAL INTERFACE)


# If you are unsure where cmake is adding sources for building
message(STATUS "Adding module source file: ${CMAKE_CURRENT_LIST_DIR}/byandcounter.cpp")

# Add our source files to the lib
target_sources(usermod_uICAL INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/uICAL.c
    # ${CMAKE_CURRENT_LIST_DIR}/base.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/byandcounter.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/bysetposcounter.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/byweekdaycounter.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/calendar.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/calendarentry.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/calendariter.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/cascade.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/counter.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/datecalc.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/dateperiod.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/datestamp.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/datetime.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/epochtime.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/logging.cpp
    ${CMAKE_CURRENT_LIST_DIR}/mp_rrule.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/rrule.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/rruleiter.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/stream.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/string.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/tz.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/tzmap.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/vevent.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/veventiter.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/vline.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/vlinestream.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/vobject.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/vobjectstream.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/util.cpp
)

target_compile_options(usermod_uICAL INTERFACE
    -fexceptions
)

# Add the current directory as an include directory.
target_include_directories(usermod_uICAL INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# # Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_uICAL)

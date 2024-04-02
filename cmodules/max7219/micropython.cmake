# Create an INTERFACE library for our CPP module.
add_library(usermod_max7219 INTERFACE)

# Add our source files to the library.
target_sources(usermod_max7219 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/mp_max7219.c
    ${CMAKE_CURRENT_LIST_DIR}/max7219.c
)


target_compile_options(usermod_max7219 INTERFACE
    -fexceptions
)

# Add the current directory as an include directory.
target_include_directories(usermod_max7219 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_max7219)

# Create an INTERFACE library for our CPP module.
add_library(usermod_LEDMatrixDriver INTERFACE)

# Add our source files to the library.
target_sources(usermod_LEDMatrixDriver INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/LEDMatrixDriver.cpp
    ${CMAKE_CURRENT_LIST_DIR}/mp_LEDMatrixDriver.c
)


target_compile_options(usermod_LEDMatrixDriver INTERFACE
    -fexceptions
)

# Add the current directory as an include directory.
target_include_directories(usermod_LEDMatrixDriver INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_LEDMatrixDriver)

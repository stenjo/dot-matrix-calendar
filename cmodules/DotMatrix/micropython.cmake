# Create an INTERFACE library for our CPP module.
add_library(usermod_dotmatrix INTERFACE)

# Add our source files to the library.
target_sources(usermod_dotmatrix INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/mp_dotmatrix.c
    ${CMAKE_CURRENT_LIST_DIR}/src/max7219.c
    ${CMAKE_CURRENT_LIST_DIR}/src/matrix.c
)


target_compile_options(usermod_dotmatrix INTERFACE
    -fexceptions
)

# Add the current directory as an include directory.
target_include_directories(usermod_dotmatrix INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_dotmatrix)

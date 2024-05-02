#
# build script written by : Michael Brockus.
# github repo author: Mike Karlesky, Mark VanderVoord, Greg Williams.
#
# license: MIT
#
inc_dir = include_directories('.', '..', '../cmock/src')
lib_list = {
    'matrix': ['matrix.c' ]
    }

foreach lib, src : lib_list
    set_variable(lib + '_lib', 
        static_library(lib + '_lib', sources: src, include_directories: inc_dir))
endforeach

matrix_dep = declare_dependency(link_with: matrix_lib, include_directories: inc_dir)

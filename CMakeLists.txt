cmake_minimum_required(VERSION 3.16)
project(ft_lib)

find_package (Threads REQUIRED)

include_directories(src)
include_directories(src/header)
include_directories(src/functions)

include_directories(test)

add_executable(ft_lib
    src/header/ftlib.h
    src/functions/find_file_functions.c
    src/functions/init_functions.c
    src/functions/utility_functions.c
    test/linux_test.c
)

target_link_libraries (ft_lib ${CMAKE_THREAD_LIBS_INIT})
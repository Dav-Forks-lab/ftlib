cmake_minimum_required(VERSION 3.17)
project(ft_lib)

# Build Shared Library with source files
add_library(ft SHARED src/ftlib.c src/win_functions.c)

cmake_minimum_required(VERSION 3.12)

project(Fractional)

include(${CMAKE_CURRENT_SOURCE_DIR}/../Effects_Float/inc/CMakeLists.txt)


add_library(Fractional
    ../../../Fractional/src/fractional.c
    ../../../Fractional/src/floatpoint.c
    ../../../Fractional/src/lookup.c)

target_include_directories(Fractional PRIVATE ../../../Fractional/inc/)
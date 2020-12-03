cmake_minimum_required(VERSION 3.12)

project(Fractional)

add_compile_options(/O2 /fp:fast /arch:AVX2)

add_library(Fractional
    ../../../Fractional/src/fractional.c
    ../../../Fractional/src/floatpoint.c
    ../../../Fractional/src/lookup.c)

target_include_directories(Fractional PRIVATE ../../../Fractional/inc/)
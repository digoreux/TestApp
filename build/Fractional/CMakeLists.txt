cmake_minimum_required(VERSION 3.12)

project(Fractional)

add_library(Fractional
    ../../src/fractional.c
    ../../src/lookup.c)

target_include_directories(Fractional PRIVATE ../../include/)
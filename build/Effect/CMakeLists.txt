cmake_minimum_required(VERSION 3.12)

project(Effect)

add_library(Effect
    ../../src/effect_control.c 
    ../../src/effect_process.c)

target_include_directories(Effect PRIVATE ../../include/)
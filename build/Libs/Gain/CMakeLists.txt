cmake_minimum_required(VERSION 3.12)

project(Gain)

add_library(Gain
    ../../../Effects/src/Gain/effect_control.c 
    ../../../Effects/src/Gain/effect_process.c)

target_include_directories(Gain PRIVATE ../../../Effects/inc/)
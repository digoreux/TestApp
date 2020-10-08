cmake_minimum_required(VERSION 3.12)

project(IIR_flt)

add_library(IIR_flt
    ../../../Effects/src/IIR_flt/effect_control.c 
    ../../../Effects/src/IIR_flt/effect_process.c)


target_include_directories(IIR_flt PRIVATE ../../../Effects/inc/)
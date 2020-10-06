cmake_minimum_required(VERSION 3.12)

project(FIR_flt)

add_library(FIR_flt
    ../../src/fir_flt_control.c 
    ../../src/fir_flt_process.c)

target_include_directories(FIR_flt PRIVATE ../../include/)
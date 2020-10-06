cmake_minimum_required(VERSION 3.12)

project(FIR_fxd)

add_library(FIR_fxd
    ../../src/fir_fxd_control.c 
    ../../src/fir_fxd_process.c)

target_include_directories(FIR_fxd PRIVATE ../../include/)
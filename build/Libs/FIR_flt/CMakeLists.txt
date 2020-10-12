cmake_minimum_required(VERSION 3.12)

project(FIR_flt)

add_library(FIR_flt
    ../../../Effects/src/FIR_flt/effect_control.c 
    ../../../Effects/src/FIR_flt/effect_process.c
    ../../../Effects/src/FIR_flt/fir_flt_coeffs.c)

target_include_directories(FIR_flt PRIVATE 
    ../../../Effects/inc/
    ../../../Fractional/inc)

target_link_libraries(FIR_flt PRIVATE
	 Fractional)
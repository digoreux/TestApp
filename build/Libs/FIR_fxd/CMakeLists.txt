cmake_minimum_required(VERSION 3.12)

project(FIR_fxd)

add_library(FIR_fxd
    ../../../Effects/src/FIR_fxd/effect_control.c 
    ../../../Effects/src/FIR_fxd/effect_process.c
    ../../../Effects/src/FIR_fxd/fir_fxd_coeffs.c)

target_include_directories(FIR_fxd PRIVATE 
    ../../../Effects/inc/
    ../../../Fractional/inc)

target_link_libraries(FIR_fxd PRIVATE
	 Fractional)
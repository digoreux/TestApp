cmake_minimum_required(VERSION 3.12)

project(IIR_fxd)

add_library(IIR_fxd
    ../../../Effects/src/IIR_fxd/effect_control.c 
    ../../../Effects/src/IIR_fxd/effect_process.c)


target_include_directories(IIR_fxd PRIVATE 
     ../../../Effects/inc/
     ../../../Fractional/inc)


target_link_libraries(IIR_fxd PRIVATE
	 Fractional)
cmake_minimum_required(VERSION 3.12)

project(Comp_fxd)

add_library(Comp_fxd
    ../../../Effects/src/Comp_fxd/effect_control.c 
    ../../../Effects/src/Comp_fxd/effect_process.c)


target_include_directories(Comp_fxd PRIVATE 
     ../../../Effects/inc/
     ../../../Fractional/inc)


target_link_libraries(Comp_fxd PRIVATE
	 Fractional)
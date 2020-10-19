cmake_minimum_required(VERSION 3.12)

project(Comp_flt)

add_library(Comp_flt
    ../../../Effects/src/Comp_flt/effect_control.c 
    ../../../Effects/src/Comp_flt/effect_process.c)


target_include_directories(Comp_flt PRIVATE 
     ../../../Effects/inc/
     ../../../Fractional/inc)


target_link_libraries(Comp_flt PRIVATE
	 Fractional)
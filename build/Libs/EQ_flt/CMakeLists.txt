cmake_minimum_required(VERSION 3.12)

project(EQ_flt)

add_library(EQ_flt
    ../../../Effects/src/EQ_flt/effect_control.c 
    ../../../Effects/src/EQ_flt/effect_process.c)


target_include_directories(EQ_flt PRIVATE 
     ../../../Effects/inc/
     ../../../Fractional/inc)


target_link_libraries(EQ_flt PRIVATE
	 Fractional)
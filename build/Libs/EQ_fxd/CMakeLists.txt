cmake_minimum_required(VERSION 3.12)

project(EQ_fxd)

add_library(EQ_fxd
    ../../../Effects/src/EQ_fxd/effect_control.c 
    ../../../Effects/src/EQ_fxd/effect_process.c)


target_include_directories(EQ_fxd PRIVATE 
     ../../../Effects/inc/
     ../../../Fractional/inc)


target_link_libraries(EQ_fxd PRIVATE
	 Fractional)
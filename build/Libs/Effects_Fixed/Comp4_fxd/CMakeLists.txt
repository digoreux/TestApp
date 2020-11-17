cmake_minimum_required(VERSION 3.12)

set(LIB_NAME "Comp4_fxd")
set(HOME_PATH "../../../..")
set(EFFECT_PATH "../../../../Effects/Fixed")

project(${LIB_NAME})

add_library(${LIB_NAME}
    ${EFFECT_PATH}/Comp_fxd/src/comp_fxd_control.c 
    ${EFFECT_PATH}/Comp_fxd/src/comp_fxd_process.c 
    ${EFFECT_PATH}/Comp4_fxd/src/comp4_fxd_control.c 
    ${EFFECT_PATH}/Comp4_fxd/src/comp4_fxd_process.c)


target_include_directories(${LIB_NAME} PRIVATE 
     ${EFFECT_PATH}/Comp_fxd/inc
     ${EFFECT_PATH}/Comp4_fxd/inc
     ${HOME_PATH}/Fractional/inc)


target_link_libraries(${LIB_NAME} PRIVATE 
    Fractional
    Comp_fxd)
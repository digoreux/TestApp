cmake_minimum_required(VERSION 3.12)

set(LIB_NAME "Comp4s_flt")
set(HOME_PATH "../../../..")
set(EFFECT_PATH "../../../../Effects/Float")

project(${LIB_NAME})

add_library(${LIB_NAME}
    ${EFFECT_PATH}/Comp_flt/src/comp_flt_control.c 
    ${EFFECT_PATH}/Comp_flt/src/comp_flt_process.c 
    ${EFFECT_PATH}/Comp4s_flt/src/comp4s_flt_control.c 
    ${EFFECT_PATH}/Comp4s_flt/src/comp4s_flt_process.c)


target_include_directories(${LIB_NAME} PRIVATE 
     ${EFFECT_PATH}/inc
     ${EFFECT_PATH}/Comp_flt/inc
     ${EFFECT_PATH}/Cross_flt/inc
     ${EFFECT_PATH}/Comp4s_flt/inc
     ${HOME_PATH}/Fractional/inc)


target_link_libraries(${LIB_NAME} PRIVATE 
    Fractional
    Comp_flt)
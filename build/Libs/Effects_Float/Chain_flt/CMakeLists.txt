cmake_minimum_required(VERSION 3.12)

set(LIB_NAME "Chain_flt")
set(HOME_PATH "../../../..")
set(EFFECTS_PATH "../../../../Effects/Float")

project(${LIB_NAME})

add_library(${LIB_NAME}
    ${EFFECTS_PATH}/${LIB_NAME}/src/effect_control.c 
    ${EFFECTS_PATH}/${LIB_NAME}/src/effect_process.c)


target_include_directories(${LIB_NAME} PRIVATE 
     ${EFFECTS_PATH}/${LIB_NAME}/inc
     ${EFFECTS_PATH}/${LIB_NAME}/inc_
     ${EFFECTS_PATH}/inc
     ${EFFECTS_PATH}/EQ_flt/inc
     ${EFFECTS_PATH}/Comp_flt/inc
     ${EFFECTS_PATH}/Comp4_flt/inc
     ${EFFECTS_PATH}/Cross_flt/inc
     ${EFFECTS_PATH}/Comp4s_flt/inc
     ${EFFECTS_PATH}/Biquad_flt/inc
     ${EFFECTS_PATH}/Expand_flt/inc
     ${HOME_PATH}/Fractional/inc
     ${HOME_PATH}/inc)


target_link_libraries(${LIB_NAME} PRIVATE 
    Fractional
    EQ_flt
    Comp_flt
    Comp4_flt
    Comp4s_flt
    Expand_flt
    Biquad_flt
    Cross_flt)
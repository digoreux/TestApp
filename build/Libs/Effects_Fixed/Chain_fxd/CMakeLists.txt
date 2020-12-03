cmake_minimum_required(VERSION 3.12)

set(LIB_NAME "Chain_fxd")
set(HOME_PATH "../../../..")
set(EFFECTS_PATH "../../../../Effects/Fixed")

project(${LIB_NAME})

add_library(${LIB_NAME}
    ${EFFECTS_PATH}/${LIB_NAME}/src/effect_control.c 
    ${EFFECTS_PATH}/${LIB_NAME}/src/effect_process.c
    ${EFFECTS_PATH}/EQ_fxd/src/eq_fxd_control.c
    ${EFFECTS_PATH}/EQ_fxd/src/eq_fxd_process.c
    ${EFFECTS_PATH}/Comp_fxd/src/comp_fxd_control.c
    ${EFFECTS_PATH}/Comp_fxd/src/comp_fxd_process.c
    ${EFFECTS_PATH}/Comp4_fxd/src/comp4_fxd_control.c
    ${EFFECTS_PATH}/Comp4_fxd/src/comp4_fxd_process.c
    ${EFFECTS_PATH}/Cross_fxd/src/cross_fxd_control.c
    ${EFFECTS_PATH}/Cross_fxd/src/cross_fxd_process.c
    ${EFFECTS_PATH}/Cross_fxd/src/cross4_fxd_control.c
    ${EFFECTS_PATH}/Cross_fxd/src/cross4_fxd_process.c
    ${EFFECTS_PATH}/Biquad_fxd/src/biquad_fxd_control.c
    ${EFFECTS_PATH}/Biquad_fxd/src/biquad_fxd_process.c)


target_include_directories(${LIB_NAME} PRIVATE 
     ${EFFECTS_PATH}/${LIB_NAME}/inc
     ${EFFECTS_PATH}/${LIB_NAME}/inc_
     ${EFFECTS_PATH}/inc
     ${EFFECTS_PATH}/EQ_fxd/inc
     ${EFFECTS_PATH}/Biquad_fxd/inc
     ${EFFECTS_PATH}/Comp_fxd/inc
     ${EFFECTS_PATH}/Comp4_fxd/inc
     ${EFFECTS_PATH}/Cross_fxd/inc
     ${HOME_PATH}/Fractional/inc
     ${HOME_PATH}/inc)


target_link_libraries(${LIB_NAME} PRIVATE 
    Fractional)
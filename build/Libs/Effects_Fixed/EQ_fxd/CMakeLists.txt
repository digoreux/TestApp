cmake_minimum_required(VERSION 3.12)

set(LIB_NAME "EQ_fxd")
set(HOME_PATH "../../../..")
set(EFFECT_PATH "../../../../Effects/Fixed/${LIB_NAME}")

project(${LIB_NAME})

add_library(${LIB_NAME}
    ${EFFECT_PATH}/src/eq_fxd_control.c 
    ${EFFECT_PATH}/src/eq_fxd_process.c)


target_include_directories(${LIB_NAME} PRIVATE 
     ${EFFECT_PATH}/inc
     ${HOME_PATH}/Fractional/inc)


target_link_libraries(${LIB_NAME} PRIVATE Fractional)
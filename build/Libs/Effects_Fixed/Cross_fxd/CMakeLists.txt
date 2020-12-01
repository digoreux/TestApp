cmake_minimum_required(VERSION 3.12)

set(LIB_NAME "Cross_fxd")
set(HOME_PATH "../../../..")
set(EFFECT_PATH "../../../../Effects/Fixed")

project(${LIB_NAME})

add_library(${LIB_NAME}
    ${EFFECT_PATH}/${LIB_NAME}/src/cross_fxd_control.c 
    ${EFFECT_PATH}/${LIB_NAME}/src/cross_fxd_process.c 
    ${EFFECT_PATH}/${LIB_NAME}/src/cross4_fxd_control.c 
    ${EFFECT_PATH}/${LIB_NAME}/src/cross4_fxd_process.c)


target_include_directories(${LIB_NAME} PRIVATE 
     ${HOME_PATH}/Effects/inc
     ${EFFECT_PATH}/${LIB_NAME}/inc
     ${HOME_PATH}/Fractional/inc)


target_link_libraries(${LIB_NAME} PRIVATE Fractional)
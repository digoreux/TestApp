cmake_minimum_required(VERSION 3.12)

set(LIB_NAME "EQ_flt")
set(HOME_PATH "../../../..")
set(EFFECT_PATH "../../../../Effects/Float")

project(${LIB_NAME})

add_library(${LIB_NAME}
    ${EFFECT_PATH}/${LIB_NAME}/src/eq_flt_control.c 
    ${EFFECT_PATH}/${LIB_NAME}/src/eq_flt_process.c)


target_include_directories(${LIB_NAME} PRIVATE 
     ${EFFECT_PATH}/${LIB_NAME}/inc
     ${EFFECT_PATH}/inc
     ${HOME_PATH}/Fractional/inc)


target_link_libraries(${LIB_NAME} PRIVATE Fractional) 

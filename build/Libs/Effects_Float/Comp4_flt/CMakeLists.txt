cmake_minimum_required(VERSION 3.12)

set(LIB_NAME "Comp4_flt")
set(HOME_PATH "../../../..")
set(EFFECT_PATH "../../../../Effects/Float")

include(${CMAKE_CURRENT_SOURCE_DIR}/../inc/CMakeLists.txt)

project(${LIB_NAME})



add_library(${LIB_NAME}
    ${EFFECT_PATH}/Comp_flt/src/comp_flt_control.c 
    ${EFFECT_PATH}/Comp_flt/src/comp_flt_process.c 
    ${EFFECT_PATH}/Comp4_flt/src/comp4_flt_control.c 
    ${EFFECT_PATH}/Comp4_flt/src/comp4_flt_process.c)


target_include_directories(${LIB_NAME} PRIVATE 
     ${HOME_PATH}/Effects/inc
     ${EFFECT_PATH}/Comp_flt/inc
     ${EFFECT_PATH}/Cross_flt/inc
     ${EFFECT_PATH}/Comp4_flt/inc
     ${HOME_PATH}/Fractional/inc)


target_link_libraries(${LIB_NAME} PRIVATE 
    Fractional
    Comp_flt)
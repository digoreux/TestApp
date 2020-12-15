cmake_minimum_required(VERSION 3.12)

set(LIB_NAME "Comp_flt")
set(HOME_PATH "../../../..")
set(EFFECT_PATH "../../../../Effects/Float")

project(${LIB_NAME})

#add_compile_options(/O2 /fp:fast /arch:AVX2)

add_library(${LIB_NAME}
    ${EFFECT_PATH}/${LIB_NAME}/src/comp_flt_control.c 
    ${EFFECT_PATH}/${LIB_NAME}/src/comp_flt_process.c)


target_include_directories(${LIB_NAME} PRIVATE 
     ${HOME_PATH}/Effects/inc
     ${EFFECT_PATH}/${LIB_NAME}/inc
     ${HOME_PATH}/Fractional/inc)


target_link_libraries(${LIB_NAME} PRIVATE Fractional)
cmake_minimum_required(VERSION 3.12)

set(LIB_NAME "Chain_fxd")
set(HOME_PATH "../../../..")
set(EFFECTS_PATH "../../../../Effects/Fixed")

project(${LIB_NAME})

add_library(${LIB_NAME}
    ${EFFECTS_PATH}/${LIB_NAME}/src/chain_fxd_control.c 
    ${EFFECTS_PATH}/${LIB_NAME}/src/chain_fxd_process.c)


target_include_directories(${LIB_NAME} PRIVATE 
     ${EFFECTS_PATH}/${LIB_NAME}/inc
     ${EFFECTS_PATH}/EQ_fxd/inc
     ${EFFECTS_PATH}/Comp_fxd/inc
     ${EFFECTS_PATH}/Comp4_fxd/inc
     ${EFFECTS_PATH}/Cross_fxd/inc
     ${HOME_PATH}/Fractional/inc
     ${HOME_PATH}/inc)


target_link_libraries(${LIB_NAME} PRIVATE 
    Fractional
    EQ_fxd
    Comp_fxd
    Comp4_fxd
    Cross_fxd)
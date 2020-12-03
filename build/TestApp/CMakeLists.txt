cmake_minimum_required(VERSION 3.12)

set(LIB_NAME "TestApp")
set(HOME_PATH "../..")
set(EFFECTS_PATH "../../../../Effects/Float")

project(TestApp)

add_compile_options(/O2 /fp:fast /arch:AVX2)

#add_compile_options(-O2 -pg -msse -msse2 -msse3 -msse4.1 -mfma -msse4.2 -std=c11)
#SET(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -pg ")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
#SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")


add_executable(TestApp
    ../../src/main.c
    ../../src/file_handler.c
    ../../src/generator.c
    ../../src/arg_parser.c
    ../../src/cJSON.c)


target_link_libraries(TestApp PRIVATE
	 Chain_flt
	 Fractional)

set(EXECUTABLE_OUTPUT_PATH ../../../)

target_include_directories(TestApp PRIVATE ../../inc
	 ../../Effects/Float/Chain_flt/inc
	 ../../Fractional/inc)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

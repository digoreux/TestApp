cmake_minimum_required(VERSION 3.12)

project(TestApp)

add_executable(TestApp
    ../../src/main.c
    ../../src/file_handler.c
    ../../src/generator.c
    ../../src/arg_parser.c)

target_link_libraries(TestApp PRIVATE
	 ../../FIR_flt/tmp/Debug/FIR_flt
	 ../../Fractional/tmp/Debug/Fractional)

target_include_directories(TestApp PRIVATE ../../include)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

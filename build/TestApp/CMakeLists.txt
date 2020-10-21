cmake_minimum_required(VERSION 3.12)

project(TestApp)

add_executable(TestApp
    ../../src/main.c
    ../../src/file_handler.c
    ../../src/generator.c
    ../../src/arg_parser.c
    ../../src/cjson.c)

target_link_libraries(TestApp PRIVATE
	 EQ_fxd
	 Fractional)

target_include_directories(TestApp PRIVATE ../../inc
	 ../../Effects/inc
	 ../../Fractional/inc)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

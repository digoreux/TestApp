
cmake_minimum_required(VERSION 3.12)

project(WholeSolution)


add_subdirectory(Libs/Fractional)
add_subdirectory(Libs/Effects_Float/EQ_flt)
add_subdirectory(Libs/Effects_Float/Comp_flt)
add_subdirectory(Libs/Effects_Float/Cross_flt)
add_subdirectory(Libs/Effects_Float/Chain_flt)
add_subdirectory(TestApp)

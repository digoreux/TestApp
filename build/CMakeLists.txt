
cmake_minimum_required(VERSION 3.12)

project(WholeSolution)

#add_compile_options(/O2 /fp:fast /arch:AVX2)

add_subdirectory(Libs/Fractional)

add_subdirectory(Libs/Effects_Float/Chain_flt)
#add_subdirectory(Libs/Effects_Fixed/Chain_fxd)

add_subdirectory(TestApp)


#add_subdirectory(Libs/Effects_Float/EQ_flt)
#add_subdirectory(Libs/Effects_Float/Comp_flt)
#add_subdirectory(Libs/Effects_Float/Comp4_flt)
#add_subdirectory(Libs/Effects_Float/Cross_flt)
#add_subdirectory(Libs/Effects_Float/Comp4s_flt)
#add_subdirectory(Libs/Effects_Float/Biquad_flt)

#add_subdirectory(Libs/Effects_Fixed/EQ_fxd)
#add_subdirectory(Libs/Effects_Fixed/Comp_fxd)
#add_subdirectory(Libs/Effects_Fixed/Comp4_fxd)
#add_subdirectory(Libs/Effects_Fixed/Cross_fxd)
#add_subdirectory(Libs/Effects_Fixed/Chain_fxd)

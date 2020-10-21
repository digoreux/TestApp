TestApp

effect initialization in filehandler.c 
EQ parameters are set in Effect/eq_preset.json
Generators only support floating point
to change effect - configure CMakeLists in build and build/TestApp.

--in <path-to-file>

--out <path-to-file>

--generator sample_rate,time(sec),type:parameters

generator type:

delta      
step
square:period(ms),amp
noise:amp
sine:frequency,amp
chirp_log:start_frequency,end_frequency,amp
chirp_lin:start_frequency,end_frequency,amp
sweep:frequency, start_db, end_db

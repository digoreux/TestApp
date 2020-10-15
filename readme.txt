TestApp

effect initialization in filehandler.c read_wav(), gen_wav()
filter parameters are hardcoded via effect_set_parameter()
generators only support floating point
noise shaping aplies only to left channel in IIR_fxd
to change effect - configure CMakeLists in build and build/TestApp (IIR_fxd, IIR_flt, FIR_fxd, FIR_flt)

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

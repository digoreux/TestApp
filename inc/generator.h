#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include <stdint.h>
#include <stdio.h>                                                              
#include <stdlib.h>   
#include <math.h>
#include "arg_parser.h"
#include "effect_process.h"
#define M_PI 3.14159265358979323846


int gen_white_noise(void * buffer, size_t sample_count, float amp);

int gen_delta(void * buffer, size_t sample_count);

int gen_step(void * buffer, size_t sample_count);

int gen_square(void * buffer, size_t sample_count, float period, float amp);

int gen_sine(void * buffer, size_t sample_count, float freq, float amp, float sample_rate);

int gen_chirp_log(void * buffer, size_t sample_count, float freq_start,
    float freq_end, float amp, float sample_rate);

int gen_chirp_lin(void * buffer, size_t sample_count, float freq_start,
    float freq_end, float amp, float sample_rate);

int gen_level_sweep(void * buffer, size_t sample_count, float freq,
    float amp_start, float amp_end, float sample_rate);

void generator(void * buffer, size_t sample_count, char * type, arg_p a);

#endif // !__GENERATOR_H__
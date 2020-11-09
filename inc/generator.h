#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include <stdint.h>
#include <stdio.h>                                                              
#include <stdlib.h>   
#include <math.h>
#include "arg_parser.h"
// #include "effect_process.h"
#include "fractional.h"
#define M_PI 3.14159265358979323846

typedef enum {
    DELTA  = 0,
    STEP   = 1,
    NOISE  = 2,
    SQUARE = 3,
    SINE   = 4,
    CHRIP_LIN = 5,
    CHIRP_LOG = 6,
    LVL_SWEEP = 7,
} gen_types;

typedef struct stereo_s {
    float left;
    float right;
} stereo_t;

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

void gen_fxd(void * buffer, size_t sample_count);

#endif // !__GENERATOR_H__
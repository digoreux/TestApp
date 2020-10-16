#ifndef __ARG_PARSER_H__
#define __ARG_PARSER_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "effect_process.h"

typedef struct args_s {
    char *input;
    char *output;
    char *gen_opts;
    char *type;
    char *filter_params;

    float gaindb;
    float gain_value;
    float gain;
    float freq;
    float dbamp;
    float st_freq;
    float end_freq;
    float st_amp;
    float end_amp;
    float sample_rate;
    float period;

    size_t time;
} args_t;

typedef struct filter_params_s {
    float frequency;
    float amp;
    float Q;
    float SR;
} filter_params_t;

typedef struct args_s * arg_p;

void get_args(int argc, char ** argv, arg_p a);

#endif // !__ARG_PARSER_H__

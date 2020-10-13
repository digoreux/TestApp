#ifndef __ARG_PARSER_H__
#define __ARG_PARSER_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct args_list {
    char *input;
    char *output;
    char *gen_opts;
    char *type;

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
} args;

typedef struct args_list * arg_p;

void get_args(int argc, char ** argv, arg_p a);

#endif // !__ARG_PARSER_H__

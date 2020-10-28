#ifndef __FILE_HANDLER_H__
#define __FILE_HANDLER_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "arg_parser.h"
#include "generator.h"
#include "effect_process.h"
#include "effect_control.h"
#include "cjson.h"

typedef struct header_file 
{
    char chunk_id[4];
    uint32_t chunk_size;
    char format[4];

    char subchunk1_id[4];
    uint32_t subchunk1_size;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;

    char subchunk2_id[4];
    uint32_t subchunk2_size;

} header;

typedef struct utils_s 
{
    FILE *in;
    FILE *out;
    void * buffer;
    uint32_t buff_size;
    uint32_t num_samples;
    uint8_t reading;
    uint8_t form;         // 0=DF1, 1=DF1t, 2=DF2, 3=DF2t
} utils_t;

typedef struct utils_s* utils_p;
typedef struct header_file* header_p;

int read_header(FILE * in, header_p meta);

int write_header(FILE * out, header_p meta);

int create_header(header_p meta, float sampleRate, size_t length);

void print_header(header_p meta);

int get_fmt(header_p meta);

int apply_effect(utils_p utils);

int set_params(void * params);

int read_wav(utils_p utils, arg_p a, header_p meta);

int gen_wav(utils_p utils, arg_p a, header_p meta);

#endif // !__FILE_HANDLER_H__

#ifndef __FILE_HANDLER_H__
#define __FILE_HANDLER_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "arg_parser.h"
#include "generator.h"
#include "effect_process.h"
#include "effect_control.h"
#include <time.h>
#include "cJSON.h"

#pragma pack(push, 1)
typedef struct header_s
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

} header_t;
#pragma pack(pop)

typedef struct utils_s 
{
    FILE *in;
    FILE *out;
    void * buffer;
    uint32_t buff_size;
    uint32_t num_samples;
    uint8_t reading;
} utils_t;

int read_header(FILE * in, header_t * meta);

int write_header(FILE * out, header_t * meta);

int create_header(header_t * meta, arg_p a);

void print_header(header_t * meta);

int apply_effect(utils_t * utils);

int set_params(void * params);

int read_wav(utils_t * utils, arg_p a, header_t * meta);

int gen_wav(utils_t * utils, arg_p a, header_t * meta);

void to_fxd(void * buffer, size_t samples_count);
void to_flt(void * buffer, size_t samples_count);


#endif // !__FILE_HANDLER_H__

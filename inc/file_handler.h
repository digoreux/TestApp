#ifndef __FILE_HANDLER_H__
#define __FILE_HANDLER_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "arg_parser.h"
#include "generator.h"
#include "effect_process.h"

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

typedef struct header_file* header_p;

int read_header(FILE * in, header_p meta);

int write_header(FILE * out, header_p meta);

int create_header(header_p meta, size_t sampleRate, size_t length);

void print_header(header_p meta);

int get_fmt(header_p meta);

int copy_wav(arg_p a, header_p meta);

int gen_wav(arg_p a, header_p meta);

#endif // !__FILE_HANDLER_H__
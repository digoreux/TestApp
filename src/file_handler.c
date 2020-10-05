#include "file_handler.h"


int read_header(FILE * in, header_p meta)
{
    fread(meta->chunk_id, sizeof(char), 4, in);
    fread(&meta->chunk_size, sizeof(uint32_t), 1, in);
    fread(meta->format, sizeof(char), 4, in);

    fread(meta->subchunk1_id, sizeof(char), 4, in);
    fread(&meta->subchunk1_size, sizeof(uint32_t), 1, in);
    fread(&meta->audio_format, sizeof(uint16_t), 1, in);
    fread(&meta->num_channels, sizeof(uint16_t), 1, in);
    fread(&meta->sample_rate, sizeof(uint32_t), 1, in);
    fread(&meta->byte_rate, sizeof(uint32_t), 1, in);
    fread(&meta->block_align, sizeof(uint16_t), 1, in);
    fread(&meta->bits_per_sample, sizeof(uint16_t), 1, in);

    if (meta->subchunk1_size != 16) fseek(in, meta->subchunk1_size - 16, SEEK_CUR);

    fread(meta->subchunk2_id, sizeof(char), 4, in);
    fread(&meta->subchunk2_size, sizeof(uint32_t), 1, in);

    return 0;
}

int write_header(FILE * out, header_p meta)
{
    fwrite(meta->chunk_id, sizeof(char), 4, out);
    fwrite(&meta->chunk_size, sizeof(uint32_t), 1, out);
    fwrite(meta->format, sizeof(char), 4, out);

    fwrite(meta->subchunk1_id, sizeof(char), 4, out);
    fwrite(&meta->subchunk1_size, sizeof(uint32_t), 1, out);
    fwrite(&meta->audio_format, sizeof(uint16_t), 1, out);
    fwrite(&meta->num_channels, sizeof(uint16_t), 1, out);
    fwrite(&meta->sample_rate, sizeof(uint32_t), 1, out);
    fwrite(&meta->byte_rate, sizeof(uint32_t), 1, out);
    fwrite(&meta->block_align, sizeof(uint16_t), 1, out);
    fwrite(&meta->bits_per_sample, sizeof(uint16_t), 1, out);

    if (meta->subchunk1_size != 16) fseek(out, meta->subchunk1_size - 16, SEEK_CUR);

    fwrite(meta->subchunk2_id, sizeof(char), 4, out);
    fwrite(&meta->subchunk2_size, sizeof(uint32_t), 1, out);

    return 0;
}

int create_header(header_p meta, size_t sampleRate, size_t length)
{   
    uint32_t subchunk1_size = 16;
    uint16_t audio_format = 3;
    uint16_t num_channels = 2;
    uint32_t sample_rate = sampleRate;
    uint16_t block_align = 8;
    uint32_t byte_rate = sample_rate * block_align;
    uint16_t bits_per_sample = 32;

    size_t num_samples = sample_rate / 1000;  // samples per second
    uint32_t subchunk2_size = num_samples * length * block_align;
    uint32_t chunk_size = 36 + subchunk2_size;

    meta->chunk_id[0] = 'R';
    meta->chunk_id[1] = 'I';
    meta->chunk_id[2] = 'F';
    meta->chunk_id[3] = 'F';
    meta->format[0] = 'W';
    meta->format[1] = 'A';
    meta->format[2] = 'V';
    meta->format[3] = 'E';
    meta->subchunk1_id[0] = 'f';
    meta->subchunk1_id[1] = 'm';
    meta->subchunk1_id[2] = 't';
    meta->subchunk1_id[3] = ' ';
    meta->subchunk2_id[0] = 'd';
    meta->subchunk2_id[1] = 'a';
    meta->subchunk2_id[2] = 't';
    meta->subchunk2_id[3] = 'a';

   
    meta->chunk_size = chunk_size;
    meta->subchunk1_size = subchunk1_size;
    meta->audio_format = audio_format;
    meta->num_channels = num_channels;
    meta->sample_rate = sample_rate;
    meta->byte_rate = byte_rate;
    meta->block_align = block_align;
    meta->bits_per_sample = bits_per_sample;
    meta->subchunk2_size = subchunk2_size;


    return 0;
}

void print_header(header_p meta)
{   
    printf("Chunk id: %c%c%c%c\n", meta->chunk_id[0], meta->chunk_id[1], meta->chunk_id[2], meta->chunk_id[3]);
    printf("Chunk size: %d\n", meta->chunk_size);
    printf("Format: %c%c%c%c\n", meta->format[0], meta->format[1], meta->format[2], meta->format[3]);
    printf("==============================================\n");
    printf("Subchunk1 id: %c%c%c%c\n", meta->subchunk1_id[0], meta->subchunk1_id[1], meta->subchunk1_id[2], meta->subchunk1_id[3]);
    printf("Subchunk1 size: %d\n", meta->subchunk1_size);
    printf("Audio format: %d\n", meta->audio_format);
    printf("Num channels: %d\n", meta->num_channels);
    printf("Sample rate: %d\n", meta->sample_rate);
    printf("Byte rate: %d\n", meta->byte_rate);
    printf("Block align: %d\n", meta->block_align);
    printf("Bits per sample: %d\n", meta->bits_per_sample);
    printf("==============================================\n");
    printf("Subchunk2 id: %c%c%c%c\n", meta->subchunk2_id[0], meta->subchunk2_id[1], meta->subchunk2_id[2], meta->subchunk2_id[3]);
    printf("Subchunk2 size: %d\n", meta->subchunk2_size);
}

int get_fmt(header_p meta)
{
    if (meta->audio_format == 1) return 1;
    else return 0;
}

int copy_wav(arg_p a, header_p meta)
{
    FILE * in  = fopen(a->input, "rb");
    FILE * out = fopen(a->output, "wb");

    read_header(in, meta);
    print_header(meta);
    write_header(out, meta);

    size_t num_samples = ((meta->sample_rate / 100) * 2 );  // 10ms   
    size_t size = ((num_samples * meta->block_align) / 2);
    void *buffer = malloc(size);

    while (!feof(in))
    {
        fread(buffer, 1, size, in);
        if (meta->audio_format == 3) effect_process(&a->gain, buffer, num_samples);
        fwrite(buffer, 1, size, out);
    }
    return 1;
}

int gen_wav(arg_p a, header_p meta)
{
    FILE * out = fopen(a->output, "wb");

    create_header(meta, a->sample_rate, a->time);
    write_header(out, meta);
    print_header(meta);
    size_t num_samples = (meta->sample_rate / 1000) * a->time;
    size_t size = ((num_samples * meta->block_align) / 2);
    void *buffer = malloc(size);

    generator(buffer, num_samples, a->type, a);
    fwrite(buffer, 1, size, out);
    return 1;
}

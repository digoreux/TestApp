#include "generator.h"

typedef struct stereo_s {
    float left;
    float right;
} stereo_t;

int gen_white_noise(void * buffer, size_t sample_count, float dbamp)
{   
    stereo_t * audio = (stereo_t*)buffer;
    float amp = powf(10.0f, dbamp / 20.0f);

    for (size_t i = 0; i < sample_count; i++) 
    {   
        audio[i].left  = amp * ((float)rand() / (float)(RAND_MAX) * 2 - 1);
        audio[i].right = amp * ((float)rand() / (float)(RAND_MAX) * 2 - 1);
    }
    return 0;
}

int gen_delta(void * buffer, size_t sample_count) 
{ 
    stereo_t * audio = (stereo_t*)buffer;
    audio[0].left  = 1.0;
    audio[0].right = 1.0;

    for (size_t i = 2; i < sample_count; i++) 
    {
        audio[i].left  = 0;
        audio[i].right = 0;
    }
    return 0;
}

int gen_step(void * buffer, size_t sample_count)
{
    stereo_t * audio = (stereo_t*)buffer;
    for (size_t i = 0; i < sample_count; i++)
    {
        audio[i].left  = 1.0;
        audio[i].right = 1.0;
    }
    return 0;
}

int gen_square(void * buffer, size_t sample_count, float period, float dbamp)
{
    stereo_t * audio = (stereo_t*)buffer;
    float amp = powf(10.0f, dbamp / 20.0f);
    size_t count = 0;
    float k = 1.0f;

    for (size_t i = 0; i < sample_count; i++) 
    {
        audio[i].left  = amp * 1.0f * k;
        audio[i].right = amp * 1.0f * k;
        count++;
        if (count == period) {
            count = 0;
            k *= -1;
        }
    }
    return 0;
}

int gen_sine(void * buffer, size_t sample_count, float freq, float dbamp, float sample_rate)
{
    stereo_t * audio = (stereo_t*)buffer;
    float amp = powf(10.0f, dbamp / 20.0f);

    for (size_t i = 0; i < sample_count; i++)
    {
        audio[i].left  = amp * (sinf(2.0f * (float)M_PI * (float)i * freq / sample_rate));
        audio[i].right = amp * (sinf(2.0f * (float)M_PI * (float)i * freq / sample_rate));
    }
    return 0;
}

 
int gen_chirp_log(void * buffer, size_t sample_count, float freq_start,
    float freq_end, float dbamp, float sample_rate)
{   
    stereo_t * audio = (stereo_t*)buffer;
    float amp  = powf(10.0f, dbamp / 20.0f);
    float freq = freq_start;
    float phi  = 0.0f;
    freq_start = 2.0f * (float)M_PI * freq_start / sample_rate;
    freq_end   = 2.0f * (float)M_PI * freq_end   / sample_rate;

    for (size_t i = 0; i < sample_count; i ++) 
    {
        audio[i].left  = amp * sinf(phi);
        audio[i].right = amp * sinf(phi);

        freq = powf(10.0f, log10f(freq_start) + log10f(freq_end)
             - log10f(freq_start) * ((float)i / (float)sample_count));
        phi  = fmodf(phi + freq, 2.0f * (float)M_PI);
    }

    return 0;
}

int gen_chirp_lin(void * buffer, size_t sample_count, float freq_start,
    float freq_end, float dbamp, float sample_rate)
{   
    stereo_t * audio = (stereo_t*)buffer;
    float amp  = powf(10.0f, dbamp / 20.0f);
    float freq = freq_start;
    float phi  = 0.0f;
    freq_start = 2.0f * (float)M_PI * freq_start / sample_rate;
    freq_end   = 2.0f * (float)M_PI * freq_end   / sample_rate;

    for (size_t i = 0; i < sample_count; i++) 
    {
        audio[i].left  = amp * sinf(phi);
        audio[i].right = amp * sinf(phi);

        freq = (freq_end - freq_start) * ((float)i / (float)sample_count) + freq_start;
        phi  = fmodf(phi + freq, 2.0f * (float)M_PI);
    }

    return 0;
}


int gen_level_sweep(void * buffer, size_t sample_count, float freq,
    float amp_start, float amp_end, float sample_rate) 
{
    stereo_t * audio = (stereo_t*)buffer;
    float amp_start_c = (float)powf(10.0f, amp_start / 20.0f);
    float amp_end_c   = (float)powf(10.0f, amp_end   / 20.0f);

    float a_delta = (amp_end_c - amp_start_c) / sample_count;

    for (size_t i = 0; i < sample_count; i++)
    {
        audio[i].left  = amp_start_c * sinf(2.0f * (float)M_PI * (float)i * freq / sample_rate);
        audio[i].right = amp_start_c * sinf(2.0f * (float)M_PI * (float)i * freq / sample_rate);
        amp_start_c += a_delta;
    }
    return 0;
}

void generator(void * buffer, size_t sample_count, char * type, arg_p a)
{

    if (!strcmp(type, "delta")) {
        gen_delta(buffer, sample_count);
    }
    if (!strcmp(type, "step")) {
        gen_step(buffer, sample_count);
    }
    if (!strcmp(type, "noise")) {
        gen_white_noise(buffer, sample_count, a->dbamp);
    }
    if (!strcmp(type, "square")) {
        gen_square(buffer, sample_count, a->period, a->dbamp);
    }
    if (!strcmp(type, "sine")) {
        gen_sine(buffer, sample_count, a->freq, a->dbamp, a->sample_rate);
    }
    if (!strcmp(type, "chirp_lin")) {
        gen_chirp_lin(buffer, sample_count, a->st_freq, a->end_freq, a->dbamp, a->sample_rate);
    }
    if (!strcmp(type, "chirp_log")) {
        gen_chirp_log(buffer, sample_count, a->st_freq, a->end_freq, a->dbamp, a->sample_rate);
    }
    if (!strcmp(type, "sweep")) {
        gen_level_sweep(buffer, sample_count, a->freq, a->st_amp, a->end_amp, a->sample_rate);
    }
}

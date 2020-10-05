#include "generator.h"


int gen_white_noise(float * buffer, size_t sample_count, float dbamp)
{   
    float amp = pow(10.0, dbamp / 20.0);
    for (size_t i = 0; i < sample_count; i++) {
        ((float*)buffer)[i] = amp * ((float)rand() / (float)(RAND_MAX) * 2 - 1);
    }
    return 0;
}

int gen_delta(void * buffer, size_t sample_count) 
{ 
    ((float *)buffer)[0] = 1.0;
    ((float *)buffer)[1] = 1.0;
    for (size_t i = 2; i < sample_count; i++) {
        ((float *)buffer)[i] = 0;
    }
    return 0;
}

int gen_step(void * buffer, size_t sample_count)
{
    for (size_t i = 0; i < sample_count; i++) {
        ((float *)buffer)[i] = 1.0;
    }
    return 0;
}

int gen_square(void * buffer, size_t sample_count, size_t period, float dbamp)
{
    size_t count = 0;
    float k = 1.0;
    float amp = pow(10.0, dbamp / 20.0);
    for (size_t i = 0; i < sample_count; i++) {
        ((float *)buffer)[i] = 1.0*k;
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
    float amp = pow(10.0, dbamp / 20.0);
    for (size_t i = 0; i < sample_count; i++) {
        ((float *)buffer)[i] = amp *  (sinf(M_PI * (float)i * freq / sample_rate));
    }
    return 0;
}

 
int gen_chirp_log(void * buffer, size_t sample_count, float freq_start,
    float freq_end, float dbamp, float sample_rate)
{   
    float amp = pow(10.0, dbamp / 20.0);
    freq_start = 2.0 * M_PI * freq_start / sample_rate;
    freq_end = 2.0 * M_PI * freq_end / sample_rate;
    float phi = 0;
    float freq = freq_start;

    for (size_t i = 0; i < sample_count; i += 2) {

        ((float *)buffer)[i] = amp * sinf(phi);
        ((float *)buffer)[i + 1] = amp * sinf(phi);
        freq = powf(10.0, log10f(freq_start) + log10f(freq_end)
            - log10f(freq_start) * ((float)i / (float)sample_count));
        phi = fmodf(phi + freq, 2.0 * M_PI);
    }

    return 0;
}

int gen_chirp_lin(void * buffer, size_t sample_count, float freq_start,
    float freq_end, float dbamp, float sample_rate)
{   
    float amp = pow(10.0, dbamp / 20.0);
    freq_start = 2.0 * M_PI * freq_start / sample_rate;
    freq_end = 2.0 * M_PI * freq_end / sample_rate;
    float phi = 0;
    float freq = freq_start;

    for (size_t i = 0; i < sample_count; i+=2) {

        ((float *)buffer)[i] = amp * sinf(phi);
        ((float *)buffer)[i+1] = amp * sinf(phi);
        freq = (freq_end - freq_start) * ((float)i / (float)sample_count) + freq_start;
        phi = fmodf(phi + freq, 2.0 * M_PI);
    }

    return 0;
}


int gen_level_sweep(void * buffer, size_t sample_count, float freq,
    float amp_start, float amp_end, float sample_rate) 
{

    float amp_start_c = (float)pow(10.0, amp_start / 20.0);
    float amp_end_c = (float)pow(10.0, amp_end / 20.0);

    float a_delta = (amp_end_c - amp_start_c) / sample_count;

    for (size_t i = 0; i < sample_count; i++) {
        ((float *)buffer)[i] = amp_start_c * sinf(M_PI * i * freq / sample_rate);
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
    if (!strcmp(type, "white_noise")) {
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

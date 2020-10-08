#include "effect_process.h"
#include "effect_control.h"


typedef struct stereo {
    float left;
    float right;
} channel;


int32_t effect_process_get_sizes(
    size_t*     states_bytes)
{
    return 0;
}

int32_t effect_reset(
    void const* coeffs,
    void*       states)
{
    return 0;
}

int32_t effect_process(
    void const* coeffs,
    void*       audio,
    size_t      samples_count)
{   
    float cbuffer[128];
    float acc = 0;
    size_t index = 0;
    channel *ch = (channel*)(audio);
    memset(cbuffer, 0, sizeof(float) * 128);
    for (size_t i = 0; i < samples_count; i++)
    {
        index = (i + 128) & 127;
        cbuffer[index] = ((float*)audio)[i];
        for (size_t j = 0; j < 128; j++) {
            acc += cbuffer[index - j] * ((float*)coeffs)[index];
        }

    ((float*)audio)[i] = acc;
    }   
   
    return 0;
}


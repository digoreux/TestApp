#include "effect_process.h"

typedef struct stereo {
    float left;
    float right;
} channels;

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
    void*       states,
    void*       audio,
    size_t      samples_count)
{   
    channels *ch = (channels*)(audio);
    for(size_t i = 0; i < samples_count; i++) {   
        ch[i].left  *= *(float*)coeffs;
        //ch[i].right *= *(float*)coeffs;
    }
    return 0;
}
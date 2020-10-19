#include "effect_process.h"
#include "fractional.h"

typedef struct stereo_s {
    flt left;
    flt right;
} stereo_t;

typedef struct states_s {
    stereo_t gain;
    stereo_t rms;
} states_t;

int32_t effect_process_get_sizes(
    size_t*     states_bytes)
{   
    *states_bytes = sizeof(states_t);
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
    stereo_t * a = (stereo_t *)audio;
    for(size_t i = 0; i < samples_count; i++)
    {
        a[i].left  = 0.5f * a[i].left;
        a[i].right = 0.5f * a[i].right;
    }
    return 0;
}
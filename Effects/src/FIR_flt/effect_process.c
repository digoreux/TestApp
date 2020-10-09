#include "effect_process.h"
#include "effect_control.h"

#define NTAPS 128


typedef struct stereo_s {
    float left;
    float right;
} stereo;

typedef struct state_s {
    stereo cbuffer[NTAPS];
} states_t;


int32_t effect_process_get_sizes(
    size_t*     states_bytes)
{   
    *states_bytes = sizeof(state);
    return 0;
}

int32_t effect_reset(
    void const* coeffs,
    void*       states)
{
    states_t * st = (states_t*)states;
    memset(st->cbuffer, 0, sizeof(st->cbuffer));
    return 0;
}

int32_t effect_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{   
    double racc;
    double lacc;
    size_t index = -1;

    stereo    *au = (stereo*)(audio);
    states_t  *st = (states_t*)(states);
     

    for (size_t i = 0; i < samples_count; i++)
    {
        index = (i + 128) & 127;
        st->cbuffer[index] = au[i];
        racc = 0;
        lacc = 0;
        for (size_t j = 0; j <= 128; j++) {
            lacc += st->cbuffer[(index + j) & 127].left  * ((double*)coeffs)[j];
            racc += st->cbuffer[(index + j) & 127].right * ((double*)coeffs)[j];
        }

        au[i].left  = lacc;
        au[i].right = racc;
    }   
   
    return 0;
}


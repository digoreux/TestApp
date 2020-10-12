#include "effect_process.h"
#include "effect_control.h"

#include "fractional.h"

#define NTAPS 128


typedef struct stereo_t_s {
    flt left;
    flt right;
} stereo_t;

typedef struct state_s {
    stereo_t cbuffer[NTAPS];
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
    flt racc;
    flt lacc;
    size_t index = -1;
    size_t index0 = 0;
    stereo_t  *au = (stereo_t*)(audio);
    states_t  *st = (states_t*)(states);
     

    for (size_t i = 0; i < samples_count; i++)
    { 
        index = i & 127;
        st->cbuffer[index] = au[i];
        racc = 0;
        lacc = 0;
        for (size_t j = 0; j < 128; j++) {
            index0 = (index + j) & 127;
            lacc = macf(st->cbuffer[index0].left,  ((flt*)coeffs)[j], lacc);
            racc = macf(st->cbuffer[index0].right, ((flt*)coeffs)[j], racc);
        }

        au[i].left  = lacc;
        au[i].right = racc;
    }   
   
    return 0;
}


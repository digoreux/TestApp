#include "effect_process.h"
#include "effect_control.h"

#include "fractional.h"

#define NTAPS 128
#define MASK  (NTAPS - 1)


typedef struct stereo_s {
    q31 left;
    q31 right;
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
    q63 racc;
    q63 lacc;
    size_t index = -1;
    size_t index0 = 0;
    stereo_t  *au = (stereo_t*)(audio);
    states_t  *st = (states_t*)(states);


    for (size_t i = 0; i < samples_count; i++)
    {
        index = i & MASK;
        st->cbuffer[index] = au[i];
        racc = 0;
        lacc = 0;
        for (size_t j = 0; j < NTAPS; j++) 
        {
            index0 = (index + j) & MASK;
            lacc = mac_q31(st->cbuffer[index0].left , ((q31*)coeffs)[j], lacc);
            racc = mac_q31(st->cbuffer[index0].right, ((q31*)coeffs)[j], racc);
        }

        au[i].left  = gethigh(lacc);
        au[i].right = gethigh(racc);
    }

    return 0;
}


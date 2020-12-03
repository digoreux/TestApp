#include "cross4_fxd_process.h"
// #include "cross_flt_process.h"

int32_t cross4_process_get_sizes(
    size_t*     states_bytes)
{
    *states_bytes = sizeof(cross4_states_t);
    return 0;
}

int32_t cross4_reset(
    void const* coeffs,
    void*       states)
{
    cross4_states_t * s = (cross4_states_t*)states;
    cross4_coeffs_t * c = (cross4_coeffs_t*)coeffs;

    cross_reset(&c->cross[0], &s->cross[0]);
    cross_reset(&c->cross[1], &s->cross[1]);
    cross_reset(&c->cross[2], &s->cross[2]);
    
    memset(s->b1, 0, 32);
    memset(s->b2, 0, 32);
    memset(s->b3, 0, 32);
    memset(s->b4, 0, 32);
    
    for(uint32_t i = 0; i < 2; i++)
    {
        s->y0[i].left = 0;
        s->x0[i].left = 0;
        s->x1[i].left = 0;
        s->y1[i].left = 0;
        s->y0[i].right = 0;
        s->x0[i].right = 0;
        s->x1[i].right = 0;
        s->y1[i].right = 0;

    }

    return 0;
}

int32_t cross4_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{   
    cross4_coeffs_t* c = (cross4_coeffs_t*)coeffs;
    cross4_states_t* s = (cross4_states_t*)states;
    stereo_t*  a = (stereo_t*)audio;
    if(!c->bypass)
    {   
        cross_process(&c->cross[1], &s->cross[1], audio, s->b1, s->b3, samples_count);
        phase_correction(c, s, s->b1, s->b3, samples_count);
        // mix2(audio, s->b1, s->b3, samples_count);
        cross_process(&c->cross[0], &s->cross[0], s->b1, s->b1, s->b2, samples_count);
        cross_process(&c->cross[2], &s->cross[2], s->b3, s->b3, s->b4, samples_count);
        mix(audio, s, samples_count);
    }

    return 0;
}


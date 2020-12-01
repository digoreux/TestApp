#include "cross_fxd_process.h"

int32_t cross_process_get_sizes(
    size_t*     states_bytes)
{
    *states_bytes = sizeof(cross_states_t);
    return 0;
}

int32_t cross_reset(
    void const* coeffs,
    void*       states)
{
    cross_states_t * s = (cross_states_t*)states;

    s->xn.left  = 0;
    s->xn.right = 0;

    for(int i = 0; i < 3; i++)
    {   
        s->y0[i].left = 0;
        s->x0[i].left = 0;
        s->x1[i].left = 0;

        s->y0[i].right = 0;
        s->x0[i].right = 0;
        s->x1[i].right = 0;
    }
    return 0;
}

int32_t cross_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    stereo_t*   band1,
    stereo_t*   band2,
    size_t      samples_count)
{   
    cross_coeffs_t* c = (cross_coeffs_t*)coeffs;
    cross_states_t* s = (cross_states_t*)states;
    stereo_t* a = (stereo_t*)audio;

    for(uint32_t i = 0; i < samples_count; i++)
    {   

 
    }   
    return 0;
}


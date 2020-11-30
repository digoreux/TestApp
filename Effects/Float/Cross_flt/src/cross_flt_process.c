#include "cross_flt_process.h"

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

    set_val(&s->xn, 0.0f);
    for(int i = 0; i < 2; i++)
    {   
        set_val(&s->y0[i], 0.0f);
        set_val(&s->x0[i], 0.0f);
        set_val(&s->x1[i], 0.0f);
    }

    return 0;
}

int32_t cross_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    stereo_t*   band1,
    stereo_t*   band2,
    size_t      samples_count,
    size_t      frames_count,
    bool        frames_mode)
{   
    cross_coeffs_t* c = (cross_coeffs_t*)coeffs;
    cross_states_t* s = (cross_states_t*)states;
    stereo_t      * a = (stereo_t*)audio;

    uint32_t n = 0;

    if(!frames_mode)
        n = samples_count * frames_count;

    vector_t y1, y2, tmp;
    
    for(uint32_t i = 0 + n, j = 0; i < samples_count + n, j < samples_count; i++, j++)
    {   
        y1 = apf1(c, s, &a[i]);
        y2 = apf2(c, s, &a[i]);

        tmp = mul2(add2(y1, y2), c->k05);
        band1[j].left  = tmp.val[3];
        band1[j].right = tmp.val[2];

        tmp = mul2(sub2(y2, y1), c->k05);
        band2[j].left  = tmp.val[3];
        band2[j].right = tmp.val[2];
    }   
    return 0;

}


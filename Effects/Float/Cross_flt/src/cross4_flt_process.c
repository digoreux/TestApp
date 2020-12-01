#include "cross4_flt_process.h"

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
    for(uint32_t i = 0; i < 3; i++)
    {
        set_val(&s->y0[i], 0.0f);
        set_val(&s->x0[i], 0.0f);
        set_val(&s->x1[i], 0.0f);
        set_val(&s->xn[i], 0.0f);
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
        phase_correction(c, s, samples_count);
        // cross_process(&c->cross[0], &s->cross[0], s->b1, s->b1, s->b2, samples_count);
        // cross_process(&c->cross[2], &s->cross[2], s->b3, s->b3, s->b4, samples_count);
        cross4_process2(c, s, s->b1, s->b3, samples_count);
    }

    return 0;
}

int32_t cross4_process2(
    void const* coeffs,
    void*       states,
    stereo_t*   band1,
    stereo_t*   band2,
    size_t      samples_count)

{
    cross4_coeffs_t* c = (cross4_coeffs_t*)coeffs;
    cross4_states_t* s = (cross4_states_t*)states;
    vector_t y1, y2, tmp;
    for(size_t i = 0; i < samples_count; i++)
    {
        y1 = apff1(c, s, &band1[i], &band2[i]);
        y2 = apff2(c, s, &band1[i], &band2[i]);

        tmp = mul2(add2(y1, y2), c->cross->k05);
        s->b1[i].left  = tmp.val[3];                    
        s->b1[i].right = tmp.val[1];                    
        s->b3[i].left  = tmp.val[2];                    
        s->b3[i].right = tmp.val[0];                    

        tmp = mul2(sub2(y2, y1), c->cross->k05);
        s->b4[i].left  = tmp.val[3];                    
        s->b4[i].right = tmp.val[1];                    
        s->b2[i].left  = tmp.val[2];                    
        s->b2[i].right = tmp.val[0];                    
    }
    return 0;
}


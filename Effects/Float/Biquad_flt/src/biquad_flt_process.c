﻿#include "biquad_flt_process.h"

int32_t bq_process_get_sizes(
    size_t*     states_bytes)
{   
    *states_bytes = sizeof(bq_states_t);
    return 0;
}

int32_t bq_reset(
    void const* coeffs,
    void*       states)
{   
    bq_states_t * s = (bq_states_t*)states;

    set_val(&s->x0, 0.0f);
    set_val(&s->x1, 0.0f);
    set_val(&s->x2, 0.0f);
    set_val(&s->y0, 0.0f);
    set_val(&s->y1, 0.0f);
    set_val(&s->y2, 0.0f);
    
    return 0;
}

int32_t bq_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{   
    bq_coeffs_t *c = (bq_coeffs_t*)coeffs;
    bq_states_t *s = (bq_states_t*)states;
    stereo_t   * a = (stereo_t *)audio;
    float acc = 0;
    double dacc = 0;
    if(1)
    {
        for (size_t i = 0; i < samples_count; i++)
        {      
            s->x0.val[3] = a[i].left;
            s->x0.val[2] = a[i].right;

            s->y0 = fma2(c->b0, s->x0, s->x1);
            
            s->x1 = fma2(c->b1, s->x0, s->x2);
            s->x1 = fma2(c->a1, s->y0, s->x1);
            s->x2 = mul2(c->b2, s->x0);
            s->x2 = fma2(c->a2, s->y0, s->x2);

            a[i].left  = s->y0.val[3];
            a[i].right = s->y0.val[2];

        }
    }
    return 0;
}



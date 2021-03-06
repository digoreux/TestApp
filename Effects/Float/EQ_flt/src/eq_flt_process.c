﻿#include "eq_flt_process.h"
#include "eq_flt_control.h"

int32_t eq_process_get_sizes(
    size_t*     states_bytes)
{   
    *states_bytes = sizeof(eq_states_t);
    return 0;
}

int32_t eq_reset(
    void const* coeffs,
    void*       states)
{   
    eq_states_t * s = (eq_states_t*)states;
    eq_coeffs_t * c = (eq_coeffs_t*)coeffs;
    
    for(uint32_t i = 0; i < 10; i++)
        bq_reset(&c->bq[i], &s->bq[i]);
    return 0;
}

int32_t eq_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{   
    eq_coeffs_t *c = (eq_coeffs_t*)coeffs;
    eq_states_t *s = (eq_states_t*)states;

    if(!c->bypass)
    {
        for(size_t j = 0; j < 10; j++)
        {   
            bq_process(&c->bq[j], &s->bq[j], audio, samples_count);
        } 
    }
    return 0;
}


#include "comp_flt_process.h"


int32_t comp_process_get_sizes(
    size_t*     states_bytes)
{
    *states_bytes = sizeof(comp_states_t);

    return 0;
}

int32_t comp_reset(
    void const* coeffs,
    void*       states)
{
    comp_states_t* s = (comp_states_t*)states;

    s->gc   = 0.0f;
    s->gm   = 0.0f;
    s->gs0  = 0.0f;
    s->gs1  = 0.0f;
    s->env0 = 0.0f;
    s->env1 = 0.0f;

    return 0;
}


int32_t comp_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{
    comp_coeffs_t* c = (comp_coeffs_t*)coeffs;
    comp_states_t* s = (comp_states_t*)states;
    stereo_t* a = (stereo_t*)audio;
    float abs;
    if(!c->bypass)
    {
        for (size_t i = 0; i < samples_count; i++)
        {
            abs = fmaxf(fabsf(a[i].left), fabsf(a[i].right));  


            if (abs < c->thrsh)
                s->gc = 1.0f;
            else
                s->gc = powf(divf(c->thrsh, abs), (1.0 - (1.0 / c->ratio)));

            if (s->gc > s->gs1)
            {
                s->gs0 = mulf(c->gainR, s->gs1);   
                s->gs0 = macf(1.0f - c->gainR, s->gc, s->gs0); 
            }
            else
            {   
                s->gs0 = mulf(c->gainA, s->gs1);   
                s->gs0 = macf(1.0f - c->gainA, s->gc, s->gs0);   
            }
    
            s->gs1 = s->gs0;
            s->gm  = s->gs0 * c->gainM;          
            
            // a[i].left  = s->gs0;
            // a[i].left = s->env0;
            a[i].left  *= s->gm;
            a[i].right *= s->gm;
            
        }
    }

    return 0;
}


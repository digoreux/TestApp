#include "comp_fxd_process.h"

#define ONE 0x7FFFFFFF

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

    s->gc   = 0;
    s->gm   = 0;
    s->gs0  = 0;
    s->gs1  = ONE;
    s->env0 = 0;
    s->env1 = 0;

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
    q31 abs = 0;
    if(!c->bypass)
    {
        for (size_t i = 0; i < samples_count; i++)
        {
            abs = max(abs_q31(a[i].left), abs_q31(a[i].right));

            /* Envelope detector */

            if (abs > s->env1)             
            {
                s->env0 = mul_q31(c->envA, s->env1);          
                s->env0 = mac_q31(sub_q31(ONE, c->envA), abs, s->env0); 
            }
            else
            {
                s->env0 = mul_q31(c->envR, s->env1);          
                s->env0 = mac_q31(sub_q31(ONE, c->envR), abs, s->env0); 
            }

            s->env1 = s->env0;

            /* Gain computer */

            if (s->env0 < c->thrsh)
            {   
                s->gc = ONE;
            }
            else
            {   
                s->gc = div_q31(c->thrsh, s->env1);
                s->gc = pow_q31(s->gc, c->ratio);
            }

            /* Gain smoothing */

            if (s->gc <= s->gs1)
            {
                s->gs0 = mul_q31(c->envA, s->gs1);          
                s->gs0 = mac_q31(sub_q31(ONE, c->gainA), s->gc, s->gs0); 
            }
            else
            {
                s->gs0 = mul_q31(c->envR, s->gs1);          
                s->gs0 = mac_q31(sub_q31(ONE, c->gainR), s->gc, s->gs0); 
            }
    
            s->gs1 = s->gs0;
            s->gm  = gethigh(left_shift_q63(mul_q63(s->gc, c->gainM), 4));

            a[i].left  = mul_q31(s->gm, a[i].left);
            a[i].right = mul_q31(s->gm, a[i].right);
        }
    }
    return 0;
}


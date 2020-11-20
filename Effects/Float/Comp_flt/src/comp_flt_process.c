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

    s->gc   = 0.0;
    s->gm   = 0.0;
    s->gs0  = 0.0;
    s->gs1  = 1.0;
    s->env0 = 0.0;
    s->env1 = 0.0;

    //s->mgc.v   = _mm_set_ps(s->gc.left, s->gc.right, 0.0f, 0.0f);
    //s->mgm.v   = _mm_set_ps(s->gm.left, s->gm.right, 0.0f, 0.0f);
    //s->mgs0.v  = _mm_set_ps(s->gs0.left, s->gs0.right, 0.0f, 0.0f);
    //s->mgs1.v  = _mm_set_ps(s->gs1.left, s->gs1.right, 0.0f, 0.0f);
    //s->menv0.v = _mm_set_ps(s->env0.left, s->env0.right, 0.0f, 0.0f);
    //s->menv1.v = _mm_set_ps(s->env1.left, s->env1.right, 0.0f, 0.0f); 

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

    float x_abs;
    if(!c->bypass)
    {
    for (uint32_t i = 0; i < samples_count; i++)
        {
            x_abs = fmaxf(fabsf(a[i].left), fabsf(a[i].right));  

            /* Envelope detector */

            if (x_abs > s->env1)             
            {
                s->env0 = mulf(c->envA, s->env1);          
                s->env0 = macf(1.0f - c->envA, x_abs, s->env0);     
            }
            else
            {
                s->env0 = mulf(c->envR, s->env1);          
                s->env0 = macf(1.0f - c->envR, x_abs, s->env0);             
            }

            s->env1 = s->env0;

            /* Gain computer */

            if (s->env0 < c->thrsh)
            {   
                s->gc = 1;
            }
            else
            {   
                s->gc = powf(divf(s->env0, c->thrsh), divf(1.0f,  c->ratio));
                s->gc = mulf(s->gc, c->thrsh);
                s->gc = divf(s->gc, s->env0);
            }

            /* Gain smoothing */

            if (s->gc <= s->gs1)
            {
                s->gs0 = mulf(c->gainA, s->gs1);   
                s->gs0 = macf(1.0f - c->gainA, s->gc, s->gs0);   
            }
            else
            {   
                s->gs0 = mulf(c->gainR, s->gs1);   
                s->gs0 = macf(1.0f - c->gainR, s->gc, s->gs0); 
            }
    
            s->gs1 = s->gs0;
            s->gm  = s->gs0 * c->gainM;          
            
            a[i].left  *= s->gm;
            a[i].right *= s->gm;
            
        }
    }

    return 0;
}


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

    return 0;
}


int32_t comp_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count,
    size_t      frames_count)
{
    comp_coeffs_t* c = (comp_coeffs_t*)coeffs;
    comp_states_t* s = (comp_states_t*)states;
    stereo_t* a = (stereo_t*)audio;
    uint32_t n = samples_count * frames_count;
    float x_abs;
    if(!c->bypass)
    {
        for (size_t i = 0 + n; i < samples_count + n; i++)
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
                // printf("env: %f \n", s->env0);
                // printf(" gc: %f \n\n", s->gc);
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
            
            // a[i].left  *= s->gm;
            a[i].left  = s->env0;
            // a[i].left  = s->gc;
            a[i].right *= s->gm;
            
        }
    }

    return 0;
}


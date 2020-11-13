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

    s->g_c.left  = 0.0;
    s->g_m.left  = 0.0;
    s->g_s0.left = 0.0;
    s->g_s1.left = 1.0;
    s->env0.left = 0.0;
    s->env1.left = 0.0;

    s->g_c.right  = 0.0;
    s->g_m.right  = 0.0;
    s->g_s0.right = 0.0;
    s->g_s1.right = 1.0;
    s->env0.right = 0.0;
    s->env1.right = 0.0;

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
    comp_stereo_t* a = (comp_stereo_t*)audio;

    float x_abs;

    for (uint32_t i = 0; i < samples_count; i++)
    {
        x_abs = fabsf(a[i].left);

        /* Envelope detector */

        if (x_abs > s->env1.left)             
        {
            s->env0.left = mulf(c->envA, s->env1.left);          
            s->env0.left = macf(1.0f - c->envA, x_abs, s->env0.left);     
        }
        else
        {
            s->env0.left = mulf(c->envR, s->env1.left);          
            s->env0.left = macf(1.0f - c->envR, x_abs, s->env0.left);             
        }

        s->env1.left = s->env0.left;

        /* Gain computer */

        if (s->env0.left < c->thrsh)
        {   
            s->g_c.left = 1;
        }
        else
        {   
            s->g_c.left = powf(divf(s->env0.left, c->thrsh), divf(1.0f,  c->ratio));
            s->g_c.left = mulf(s->g_c.left, c->thrsh);
            s->g_c.left = divf(s->g_c.left, s->env0.left);
        }
        // printf("Gain: %f\n", s->g_c.left);
        /* Gain smoothing */

        if (s->g_c.left <= s->g_s1.left)
        {
            s->g_s0.left = mulf(c->gainA, s->g_s1.left);   
            s->g_s0.left = macf(1.0f - c->gainA, s->g_c.left, s->env0.left);   
        }
        else
        {   
            s->g_s0.left = mulf(c->gainR, s->g_s1.left);   
            s->g_s0.left = macf(1.0f - c->gainR, s->g_c.left, s->env0.left); 
        }
 
        s->g_s1.left = s->g_s0.left;
        s->g_m.left  = s->g_c.left * c->gainM;

        /* 
        
            RIGHT CHANNEL 
        
        */

        x_abs = fabsf(a[i].right);

        /* Envelope detector */

        if (x_abs > s->env1.right)             
        {
            s->env0.right = mulf(c->envA, s->env1.right);          
            s->env0.right = macf(1.0f - c->envA, x_abs, s->env0.right);     
        }
        else
        {
            s->env0.right = mulf(c->envR, s->env1.right);          
            s->env0.right = macf(1.0f - c->envR, x_abs, s->env0.right);             
        }

        s->env1.right = s->env0.right;

        /* Gain computer */

        if (s->env0.right < c->thrsh)
        {
            s->g_c.right = 1;
        }
        else
        {   
            s->g_c.right = powf(divf(s->env0.right, c->thrsh), divf(1.0f,  c->ratio));
            s->g_c.right = mulf(s->g_c.right, c->thrsh);
            s->g_c.right = divf(s->g_c.right, s->env0.right);
        }

        /* Gain smoothing */

        if (s->g_c.right <= s->g_s1.right)
        {
            s->g_s0.right = mulf(c->gainA, s->g_s1.right);   
            s->g_s0.right = macf(1.0f - c->gainA, s->g_c.right, s->env0.right);   
        }
        else
        {   
            s->g_s0.right = mulf(c->gainR, s->g_s1.right);   
            s->g_s0.right = macf(1.0f - c->gainR, s->g_c.right, s->env0.right); 
        }

        s->g_s1.right = s->g_s0.right;
        s->g_m.right  = s->g_c.right * c->gainM;

        a[i].left  *= s->g_m.left;
        a[i].right *= s->g_m.right;

    }
    return 0;
}


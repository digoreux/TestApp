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

    s->g_c.left  = 0;
    s->g_m.left  = 0;
    s->g_s0.left = 0;
    s->g_s1.left = 0x7FFFFFFF;
    s->env0.left = 0;
    s->env1.left = 0;

    s->g_c.right  = 0;
    s->g_m.right  = 0;
    s->g_s0.right = 0;
    s->g_s1.right = 0x7FFFFFFF;
    s->env0.right = 0;
    s->env1.right = 0;

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

    q31 x_abs = 0;

    if(!c->bypass)
    {
        for (uint32_t i = 0; i < samples_count; i++)
        {
            x_abs = abs_q31(a[i].left);

            /* Envelope detector */

            if (x_abs > s->env1.left)             
            {
                s->env0.left = mul_q31(c->envA, s->env1.left);          
                s->env0.left = mac_q31(sub_q31(ONE, c->envA), x_abs, s->env0.left); 
            }
            else
            {
                s->env0.left = mul_q31(c->envR, s->env1.left);          
                s->env0.left = mac_q31(sub_q31(ONE, c->envR), x_abs, s->env0.left); 
            }

            s->env1.left = s->env0.left;

            /* Gain computer */

            if (s->env0.left < c->thrsh)
            {   
                s->g_c.left = ONE;
            }
            else
            {   
                s->g_c.left = div_q31(c->thrsh, s->env1.left);
                s->g_c.left = pow_q31(s->g_c.left, c->ratio);
            }

            /* Gain smoothing */

            if (s->g_c.left <= s->g_s1.left)
            {
                s->g_s0.left = mul_q31(c->envA, s->g_s1.left);          
                s->g_s0.left = mac_q31(sub_q31(ONE, c->gainA), s->g_c.left, s->g_s0.left); 
            }
            else
            {
                s->g_s0.left = mul_q31(c->envR, s->g_s1.left);          
                s->g_s0.left = mac_q31(sub_q31(ONE, c->gainR), s->g_c.left, s->g_s0.left); 
            }
    
            s->g_s1.left = s->g_s0.left;
            s->g_m.left  = gethigh(left_shift_q63(mul_q63(s->g_c.left, c->gainM), 4));
            a[i].left  = mul_q31(s->g_m.left, a[i].left);

            /* RIGHT CHANNEL */

            x_abs = abs_q31(a[i].right);

            /* Envelope detector */

            if (x_abs > s->env1.right)             
            {
                s->env0.right = mul_q31(c->envA, s->env1.right);          
                s->env0.right = mac_q31(sub_q31(ONE, c->envA), x_abs, s->env0.right); 
            }
            else
            {
                s->env0.right = mul_q31(c->envR, s->env1.right);          
                s->env0.right = mac_q31(sub_q31(ONE, c->envR), x_abs, s->env0.right); 
            }

            s->env1.right = s->env0.right;

            /* Gain computer */

            if (s->env0.right < c->thrsh)
            {   
                s->g_c.right = ONE;
            }
            else
            {   
                s->g_c.right = div_q31(c->thrsh, s->env1.right);
                s->g_c.right = pow_q31(s->g_c.right, c->ratio);
            }

            /* Gain smoothing */

            if (s->g_c.right <= s->g_s1.right)
            {
                s->g_s0.right = mul_q31(c->envA, s->g_s1.right);          
                s->g_s0.right = mac_q31(sub_q31(ONE, c->gainA), s->g_c.right, s->g_s0.right); 
            }
            else
            {
                s->g_s0.right = mul_q31(c->envR, s->g_s1.right);          
                s->g_s0.right = mac_q31(sub_q31(ONE, c->gainR), s->g_c.right, s->g_s0.right); 
            }
    
            s->g_s1.right = s->g_s0.right;
            s->g_m.right  = gethigh(left_shift_q63(mul_q63(s->g_c.right, c->gainM), 4));
            a[i].right  = mul_q31(s->g_m.right, a[i].right);

        }
    }

    return 0;
}


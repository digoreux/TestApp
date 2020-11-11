#include "comp_flt_process.h"


int32_t comp_effect_process_get_sizes(
    size_t*     states_bytes)
{
    *states_bytes = sizeof(comp_states_t);
}



int32_t comp_effect_reset(
    void const* coeffs,
    void*       states)
{
    comp_states_t* s = (comp_states_t*)states;

    s->x_sc.left = 0.0;
    s->g_c.left = 0.0;
    s->g_s0.left = 0.0;
    s->g_s1.left = 1.0;
    s->g_m.left = 0.0;
    s->env0.left = 0.0;
    s->env1.left = 0.0;

    s->x_sc.right = 0.0;
    s->g_c.right = 0.0;
    s->g_s0.right = 0.0;
    s->g_s1.right = 1.0;
    s->g_m.right = 0.0;
    s->env0.right = 0.0;
    s->env1.right = 0.0;

}


int32_t comp_effect_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{


    comp_coeffs_t* c = (comp_coeffs_t*)coeffs;
    comp_states_t* s = (comp_states_t*)states;
    comp_stereo_t* a = (comp_stereo_t*)audio;

    float xL_abs;
    float xR_abs;
    float axil1;

    for (uint32_t i = 0; i < samples_count; i++)
    {
        xL_abs = fabsf(a[i].left);

        if (xL_abs > s->env1.left)              // comparison of current gain and previos gain
        {

            s->env0.left = c->attackEnv * s->env1.left + (1.0 - c->attackEnv) * xL_abs;     // if current gain higher than previous -> attac
        }
        else
        {
            s->env0.left = c->releaseEnv * s->env1.left + (1.0 - c->releaseEnv) * xL_abs;    // attenuate
        }

        s->env1.left = s->env0.left;

        if (s->env0.left < c->threshold)
        {
            s->x_sc.left = s->env0.left;
            s->g_c.left = 1;
        }
        else
        {
            s->x_sc.left = c->threshold * powf((s->env0.left / c->threshold), (1.0 / c->ratio));
            s->g_c.left = s->x_sc.left / s->env0.left;
        }

        if (s->g_c.left <= s->g_s1.left)
        {
            s->g_s0.left = c->alphaAttack* s->g_s1.left + (1.0 - c->alphaAttack)*s->g_c.left;
        }
        else
        {
            s->g_s0.left = c->alphaRelease* s->g_s1.left + (1.0 - c->alphaRelease)*s->g_c.left;
        }

        s->g_s1.left = s->g_s0.left;
        s->g_m.left = s->g_s0.left * c->makeUpGain;

        /* 
        
            RIGHT CHANNEL 
        
        */

        xR_abs = fabsf(a[i].right);

        if (xR_abs > s->env1.right)              // comparison of current gain and previos gain
        {

            s->env0.right = c->attackEnv * s->env1.right + (1.0 - c->attackEnv) * xR_abs;     // if current gain higher than previous -> attac
        }
        else
        {
            s->env0.right = c->releaseEnv * s->env1.right + (1.0 - c->releaseEnv) * xR_abs;    // attenuate
        }

        s->env1.right = s->env0.right;

        if (s->env0.right < c->threshold)
        {
            s->x_sc.right = s->env0.right;
            s->g_c.right = 1;
        }
        else
        {
            s->x_sc.right = c->threshold * powf((s->env0.right / c->threshold), (1.0 / c->ratio));
            s->g_c.right = s->x_sc.right / s->env0.right;
        }

        if (s->g_c.right <= s->g_s1.right)
        {
            s->g_s0.right = c->alphaAttack * s->g_s1.right + (1.0 - c->alphaAttack)*s->g_c.right;
        }
        else
        {
            s->g_s0.right = c->alphaRelease * s->g_s1.right + (1.0 - c->alphaRelease)*s->g_c.right;
        }

        s->g_s1.right = s->g_s0.right;
        s->g_m.right = s->g_s0.right * c->makeUpGain;


        a[i].left  *= s->g_m.left;
        a[i].right *= s->g_m.right;
    }
}


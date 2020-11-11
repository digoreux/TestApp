#ifndef __EFFECT_PROCESS_H__
#define __EFFECT_PROCESS_H__

#include <stdint.h>
#include <stddef.h>
#include "fractional.h"

typedef struct coeffs_s{
    flt threshold;
    flt ratio;

    flt alphaAttack;
    flt alphaRelease;

    flt attackEnv;
    flt releaseEnv;

    flt makeUpGain;
    flt samplerate;

} coeffs_t;

typedef struct stereo_s {
    flt left;
    flt right;
} stereo_t;


typedef struct states_s{
    stereo_t x;          
    stereo_t y;

    stereo_t x_dB;       
    stereo_t x_sc;       

    stereo_t g_c;

    stereo_t g_s0;
    stereo_t g_s1;

    stereo_t g_m;

    stereo_t g_lin;

    stereo_t env0;
    stereo_t env1;

} states_t;



int32_t effect_process_get_sizes(
    size_t*     states_bytes)
{
    *states_bytes = sizeof(states_t);
}



int32_t effect_reset(
    void const* coeffs,
    void*       states)
{
    states_t* s = (states_t*)states;

    s->x.left = 0.0;
    s->y.left = 0.0;
    s->x_dB.left = 0.0;
    s->x_sc.left = 0.0;
    s->g_c.left = 0.0;
    s->g_s0.left = 0.0;
    s->g_s1.left = 1.0;
    s->g_m.left = 0.0;
    s->g_lin.left = 0.0;
    s->env0.left = 0.0;
    s->env1.left = 0.0;

    s->x.right = 0.0;
    s->y.right = 0.0;
    s->x_dB.right = 0.0;
    s->x_sc.right = 0.0;
    s->g_c.right = 0.0;
    s->g_s0.right = 0.0;
    s->g_s1.right = 1.0;
    s->g_m.right = 0.0;
    s->g_lin.right = 0.0;
    s->env0.right = 0.0;
    s->env1.right = 0.0;

}


int32_t effect_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{


    coeffs_t* c = (coeffs_t*)coeffs;
    states_t* s = (states_t*)states;
    stereo_t* a = (stereo_t*)audio;

    float xL_abs;
    float xR_abs;
    float axil1;

    for (uint32_t i = 0; i < samples_count; i++)
    {
        s->x = ((stereo_t*)a)[i];

        xL_abs = fabsf(s->x.left);

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

        /* RIGHT CHANNEL */

        xR_abs = fabsf(s->x.right);

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


        a[i].left  = s->x.left * s->g_m.left;
        a[i].right = s->x.right * s->g_m.right;
    }
}


#endif

#ifndef __EFFECT_CONTROL_H__
#define __EFFECT_CONTROL_H__

#include <stdint.h>
#include <stddef.h>
#include "math.h"
#include "fractional.h"

# define M_e    ((flt)2.71828182846)

typedef struct params_s {
    flt threshold;
    flt ratio;
    flt tauAttack;
    flt tauRelease;
    flt makeUpGain;
    flt samplerate;

    flt tauEnvAtt;
    flt tauEnvRel;
} comp_params_t;

typedef struct params_s {
    flt threshold;
    flt ratio;

    flt alphaAttack;
    flt alphaRelease;

    flt attackEnv;
    flt releaseEnv;

    flt makeUpGain;
    flt samplerate;
} comp_coeffs_t;


int32_t comp_effect_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes)
{
    *params_bytes = sizeof(comp_params_t);
    *coeffs_bytes = sizeof(comp_coeffs_t);
}


int32_t comp_effect_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate)
{
    comp_params_t* p = (comp_params_t*)params;
    comp_coeffs_t* c = (comp_coeffs_t*)coeffs;

    p->threshold = 0.0;
    p->ratio = 0.0;
    p->tauAttack = 0.0;
    p->tauRelease = 0.0;
    p->makeUpGain = 0.0;
    p->samplerate = 0.0;

    c->threshold = 0.0;
    c->ratio = 0.0;
    c->alphaAttack = 0.0;
    c->alphaRelease = 0.0;
    c->makeUpGain = 0.0;
    c->samplerate = 0.0;
}



int32_t effect_set_parameter(
    void*       params,
    int32_t     id,
    flt       value)
{
    comp_params_t* p = (comp_params_t*)params;

    switch (id)
    {
        case 0:
        {
            p->threshold = value;
            break;
        }

        case 1:
        {
            p->ratio = value;
            break;
        }

        case 2:
        {
            p->tauAttack = value;
            break;
        }

        case 3:
        {
            p->tauRelease = value;
            break;
        }

        case 4:
        {
            p->makeUpGain = value;
            break;
        }

        case 5:
        {
            p->samplerate = value;
            break;
        }

        case 6:
        {
            p->tauEnvAtt = value;
            break;
        }

        case 7:
        {
            p->tauEnvRel = value;
            break;
        }

        default:
            break;
    }
}



int32_t effect_update_coeffs(
    void const* params,
    void*       coeffs)
{
    comp_params_t* p = (comp_params_t*)params;
    comp_coeffs_t* c = (comp_coeffs_t*)coeffs;

    //c->threshold = p->threshold;
    c->threshold =  powf(10.0, (p->threshold/20.0));  //in linear
    c->ratio = p->ratio;

    c->alphaAttack  = powf(M_e, (-(log(9)) / (0.001 * p->tauAttack  * p->samplerate)));
    c->alphaRelease = powf(M_e, (-(log(9)) / (0.001 * p->tauRelease * p->samplerate)));

    c->attackEnv  = powf(M_e, (-(log(9)) / (0.001 * p->tauEnvAtt * p->samplerate)));
    c->releaseEnv = powf(M_e, (-(log(9)) / (0.001 * p->tauEnvRel * p->samplerate)));

    /*c->alphaAttack = 1 - powf(M_e, ((-2.2) / (0.001*p->tauAttack*p->samplerate)));
    c->alphaRelease = 1 - powf(M_e, ((-2.2) / (0.001*p->tauRelease*p->samplerate)));*/

    //c->makeUpGain = p->makeUpGain;
    c->makeUpGain = powf(10.0, (p->makeUpGain/20.0));
    c->samplerate = p->samplerate;
}


#endif

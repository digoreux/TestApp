#include "comp_flt_control.h"

# define M_e    2.71828182846f

int32_t comp_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes)
{
    *params_bytes = sizeof(comp_params_t);
    *coeffs_bytes = sizeof(comp_coeffs_t);

    return 0;
}


int32_t comp_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate)
{
    comp_params_t* p = (comp_params_t*)params;
    comp_coeffs_t* c = (comp_coeffs_t*)coeffs;

    p->thrsh = 0.0;
    p->ratio = 0.0;
    p->tAttack = 0.0;
    p->tRelease = 0.0;
    p->makeUpGain = 0.0;
    p->sample_rate = 0;
    p->bypass = 0;
    p->downward = 0;

    c->thrsh = 0.0;
    c->ratio = 0.0;
    c->gainA = 0.0;
    c->gainR = 0.0;
    c->gainM = 0.0;
    c->envA  = 0.0;
    c->envR  = 0.0;
    c->bypass = 0;

    return 0;
}

int32_t comp_set_parameter(
    void*       params,
    int32_t     id,
    float       value)
{
    comp_params_t* p = (comp_params_t*)params;

    switch (id)
    {
        case 41:
            p->thrsh = value;
            break;
        case 42:
            p->ratio = value;
            break;
        case 43:
            p->tAttack = value;
            break;
        case 44:
            p->tRelease = value;
            break;
        case 45:
            p->makeUpGain = value;
            break;
        case 46:
            p->sample_rate = (uint32_t)value;
            break;
        case 47:
            p->tEnvAttack = value;
            break;
        case 48:
            p->tEnvRelease = value;
            break;
        case 301:
            p->bypass = value;
            break;
        default:
            break;
    }

    return 0;
}

int32_t comp_update_coeffs(
    void const* params,
    void*       coeffs)
{
    comp_params_t* p = (comp_params_t*)params;
    comp_coeffs_t* c = (comp_coeffs_t*)coeffs;

    c->ratio  = p->ratio;
    c->bypass = p->bypass;

    c->thrsh = powf(10.0f, (p->thrsh/20.0f));  
    c->gainM = powf(10.0f, (p->makeUpGain/20.0f));

    c->gainA  = powf(M_e, (-(logf(9)) / (0.001f * p->tAttack  * p->sample_rate)));
    c->gainR  = powf(M_e, (-(logf(9)) / (0.001f * p->tRelease * p->sample_rate)));

    c->envA   = powf(M_e, (-(logf(9)) / (0.001f * p->tEnvAttack  * p->sample_rate)));
    c->envR   = powf(M_e, (-(logf(9)) / (0.001f * p->tEnvRelease * p->sample_rate)));

    //c->mratio.v = _mm_set_ps(c->ratio, c->ratio, 0.0f, 0.0f);
    //c->mthrsh.v = _mm_set_ps(c->thrsh, c->thrsh, 0.0f, 0.0f);

    //c->mgainA.v = _mm_set_ps(c->gainA, c->gainA, 0.0f, 0.0f);
    //c->mgainR.v = _mm_set_ps(c->gainR, c->gainR, 0.0f, 0.0f);

    //c->menvA.v = _mm_set_ps(c->envA, c->envA, 0.0f, 0.0f);
    //c->menvR.v = _mm_set_ps(c->envR, c->envR, 0.0f, 0.0f);

    // printf("Threshold: %f \n",      c->thrsh);
    // printf("Ratio: %f \n",          c->ratio);
    // printf("Attack: %f \n",         c->gainA);
    // printf("Release: %f \n",        c->gainR);
    // printf("MakeUpGain: %f \n",     c->gainM);
    // printf("envAttack: %f \n",      c->envA);
    // printf("envRelease: %f \n\n",   c->envR);

    return 0;
}




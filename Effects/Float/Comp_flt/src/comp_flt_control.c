#include "comp_flt_control.h"


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

    double thrsh, gainM, gainA, gainR, envA, envR;
    
    // c->ratio  = (float)(1.0 - (1.0 / p->ratio));
    c->ratio  = (float)p->ratio;
    c->bypass = p->bypass;

    thrsh = pow(10.0, (p->thrsh/20.0));  
    gainM = pow(10.0, (p->makeUpGain/20.0));
    gainA = pow(M_e, (-1.0 / (0.001 * p->tAttack  * p->sample_rate)));
    gainR = pow(M_e, (-1.0 / (0.001 * p->tRelease * p->sample_rate)));
    envA  = pow(M_e, (-1.0 / (0.001 * p->tEnvAttack  * p->sample_rate)));
    envR  = pow(M_e, (-1.0 / (0.001 * p->tEnvRelease * p->sample_rate)));

    c->thrsh = (float)thrsh; 
    c->gainM = (float)gainM; 
    c->gainA = (float)gainA; 
    c->gainR = (float)gainR; 
    c->envA  = (float)envA; 
    c->envR  = (float)envR; 

    return 0;
}




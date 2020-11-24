#include "comp4_flt_control.h"

int32_t comp4_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes)
{
    *params_bytes = sizeof(comp4_params_t);
    *coeffs_bytes = sizeof(comp4_coeffs_t);

    return 0;
}


int32_t comp4_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate)
{
    comp4_params_t* p = (comp4_params_t*)params;
    comp4_coeffs_t* c = (comp4_coeffs_t*)coeffs;
    for(uint32_t i = 0; i < 4; i++)
    {
        p->bypass[i] = 0;
        c->bypass[i] = 0;
    }

    return 0;
}

int32_t comp4_set_parameter(
    void*       params,
    int32_t     id,
    float       value)
{
    comp4_params_t* p = (comp4_params_t*)params;

    switch(id)
    {
        case 53:
            p->thrsh[0] = value;
            break;
        case 54:
            p->ratio[0] = value;
            break;
        case 55:
            p->tAttack[0] = value;
            break;
        case 56:
            p->tRelease[0] = value;
            break;
        case 57:
            p->makeUpGain[0] = value;
            break;
        case 58:
            p->sample_rate = (uint32_t)value;
            break;
        case 59:
            p->tEnvAttack[0] = value;
            break;
        case 60:
            p->tEnvRelease[0] = value;
            break;
        case 61:
            p->thrsh[1] = value;
            break;
        case 62:
            p->ratio[1] = value;
            break;
        case 63:
            p->tAttack[1] = value;
            break;
        case 64:
            p->tRelease[1] = value;
            break;
        case 65:
            p->makeUpGain[1] = value;
            break;
        case 66:
            p->sample_rate = (uint32_t)value;
            break;
        case 67:
            p->tEnvAttack[1] = value;
            break;
        case 68:
            p->tEnvRelease[1] = value;
            break;
        case 69:
            p->thrsh[1] = value;
            break;
        case 70:
            p->ratio[2] = value;
            break;
        case 71:
            p->tAttack[2] = value;
            break;
        case 72:
            p->tRelease[2] = value;
            break;
        case 73:
            p->makeUpGain[2] = value;
            break;
        case 74:
            p->sample_rate = (uint32_t)value;
            break;
        case 75:
            p->tEnvAttack[2] = value;
            break;
        case 76:
            p->tEnvRelease[2] = value;
            break;
        case 77:
            p->thrsh[3] = value;
            break;
        case 78:
            p->ratio[3] = value;
            break;
        case 79:
            p->tAttack[3] = value;
            break;
        case 80:
            p->tRelease[3] = value;
            break;
        case 81:
            p->makeUpGain[3] = value;
            break;
        case 82:
            p->sample_rate = (uint32_t)value;
            break;
        case 83:
            p->tEnvAttack[3] = value;
            break;
        case 84:
            p->tEnvRelease[3] = value;
            break;
        case 304:
            p->bypass[0] = value;
            break;
        case 305:
            p->bypass[1] = value;
            break;
        case 306:
            p->bypass[2] = value;
            break;
        case 307:
            p->bypass[3] = value;
            break;
        case 303:
            p->bypass[4] = value; // Multiband bypass
            break;
    }

    return 0;
}

int32_t comp4_update_coeffs(
    void const* params,
    void*       coeffs)
{
    comp4_params_t* p = (comp4_params_t*)params;
    comp4_coeffs_t* c = (comp4_coeffs_t*)coeffs;
    double thrsh[4], gainM[4], gainA[4], gainR[4], envA[4], envR[4];

    for(uint32_t i = 0; i < 4; i++)
    {
        c->bypass[i] = p->bypass[i];
        thrsh[i] = pow(10.0, (p->thrsh[i]/20.0));  
        gainM[i] = pow(10.0, (p->makeUpGain[i]/20.0));
        gainA[i] = pow(M_e, (-(log(9)) / (0.001 * p->tAttack[i]  * p->sample_rate)));
        gainR[i] = pow(M_e, (-(log(9)) / (0.001 * p->tRelease[i] * p->sample_rate)));
        envA[i]  = pow(M_e, (-(log(9)) / (0.001 * p->tEnvAttack[i]  * p->sample_rate)));
        envR[i]  = pow(M_e, (-(log(9)) / (0.001 * p->tEnvRelease[i] * p->sample_rate)));
    }

    set_vals2(&c->thrsh, thrsh[3], thrsh[2], thrsh[1], thrsh[0]); 
    set_vals2(&c->gainM, gainM[3], gainM[2], gainM[1], gainM[0]); 
    set_vals2(&c->gainA, gainA[3], gainA[2], gainA[1], gainA[0]); 
    set_vals2(&c->gainR, gainR[3], gainR[2], gainR[1], gainR[0]); 
    set_vals2(&c-> envA,  envA[3],  envA[2],  envA[1],  envA[0]); 
    set_vals2(&c-> envR,  envR[3],  envR[2],  envR[1],  envR[0]);  
    set_val(&c->one, 1.0f);

    return 0;
}




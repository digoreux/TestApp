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
    for(uint32_t i = 0; i < 5; i++)
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
            p->thrsh[2] = value;
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
    double ratio[4], thrsh[4], gainM[4], gainA[4], gainR[4], envA[4], envR[4];
    for(uint32_t i = 0; i < 4; i++)
    {
        c->bypass[i] = p->bypass[i];

        ratio[i] = (1.0 - (1.0 / p->ratio[i]));
        thrsh[i] = pow(10.0, (p->thrsh[i]/20.0));  
        gainM[i] = pow(10.0, (p->makeUpGain[i]/20.0));
        gainA[i] = pow(M_e, (-(log(9)) / (0.001 * p->tAttack[i]  * p->sample_rate)));
        gainR[i] = pow(M_e, (-(log(9)) / (0.001 * p->tRelease[i] * p->sample_rate)));
        envA[i]  = pow(M_e, (-(log(9)) / (0.001 * p->tEnvAttack[i]  * p->sample_rate)));
        envR[i]  = pow(M_e, (-(log(9)) / (0.001 * p->tEnvRelease[i] * p->sample_rate)));
    }
    set_vals2(&c->ratio, (float)p->ratio[3], (float)p->ratio[2], (float)p->ratio[1], (float)p->ratio[0]);
    set_vals2(&c->thrsh, (float)thrsh[3], (float)thrsh[2], (float)thrsh[1], (float)thrsh[0]); 
    set_vals2(&c->gainM, (float)gainM[3], (float)gainM[2], (float)gainM[1], (float)gainM[0]); 
    set_vals2(&c->gainA, (float)gainA[3], (float)gainA[2], (float)gainA[1], (float)gainA[0]); 
    set_vals2(&c->gainR, (float)gainR[3], (float)gainR[2], (float)gainR[1], (float)gainR[0]); 
    set_vals2(&c-> envA,  (float)envA[3],  (float)envA[2],  (float)envA[1],  (float)envA[0]); 
    set_vals2(&c-> envR,  (float)envR[3],  (float)envR[2],  (float)envR[1],  (float)envR[0]);  
    set_vals2(&c-> envR,  (float)ratio[3],  (float)ratio[2],  (float)ratio[1],  (float)ratio[0]);  

    set_vals2(&c->oenvA, 1.0f - c->envA.val[3],
                         1.0f - c->envA.val[2],
                         1.0f - c->envA.val[1],
                         1.0f - c->envA.val[0]);

    set_vals2(&c->oenvR, 1.0f - c->envR.val[3],
                         1.0f - c->envR.val[2],
                         1.0f - c->envR.val[1],
                         1.0f - c->envR.val[0]);

    set_vals2(&c->ogainA, 1.0f - c->gainA.val[3],
                          1.0f - c->gainA.val[2],
                          1.0f - c->gainA.val[1],
                          1.0f - c->gainA.val[0]);

    set_vals2(&c->ogainR, 1.0f - c->gainR.val[3],
                          1.0f - c->gainR.val[2],
                          1.0f - c->gainR.val[1],
                          1.0f - c->gainR.val[0]);

    // set_vals2(&c->oratio, 1.0f / c->ratio.val[3],
    //                       1.0f / c->ratio.val[2],
    //                       1.0f / c->ratio.val[1],
    //                       1.0f / c->ratio.val[0]);

    set_val(&c->one, 1.0f);
    // printv(c->thrsh);
    // printv(c->gainM);
    // printv(c->gainA);
    // printv(c->gainR);
    // printv(c-> envA);
    // printv(c-> envR);
    // printv(c->ratio);
    // printv(c->oratio);

    return 0;
}




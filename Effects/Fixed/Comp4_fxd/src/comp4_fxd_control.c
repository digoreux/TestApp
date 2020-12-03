#include "comp4_fxd_control.h"
#include "comp_fxd_control.h"

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
        comp_control_initialize(&p->comp[i], &c->comp[i], sample_rate);

    p->bypass = 0;
    c->bypass = 0;

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
            p->comp[0].thrsh = value;
            break;
        case 54:
            p->comp[0].ratio = value;
            break;
        case 55:
            p->comp[0].tAttack = value;
            break;
        case 56:
            p->comp[0].tRelease = value;
            break;
        case 57:
            p->comp[0].makeUpGain = value;
            break;
        case 58:
            p->comp[0].sample_rate = (uint32_t)value;
            break;
        case 59:
            p->comp[0].tEnvAttack = value;
            break;
        case 60:
            p->comp[0].tEnvRelease = value;
            break;
        case 61:
            p->comp[1].thrsh = value;
            break;
        case 62:
            p->comp[1].ratio = value;
            break;
        case 63:
            p->comp[1].tAttack = value;
            break;
        case 64:
            p->comp[1].tRelease = value;
            break;
        case 65:
            p->comp[1].makeUpGain = value;
            break;
        case 66:
            p->comp[1].sample_rate = (uint32_t)value;
            break;
        case 67:
            p->comp[1].tEnvAttack = value;
            break;
        case 68:
            p->comp[1].tEnvRelease = value;
            break;
        case 69:
            p->comp[2].thrsh = value;
            break;
        case 70:
            p->comp[2].ratio = value;
            break;
        case 71:
            p->comp[2].tAttack = value;
            break;
        case 72:
            p->comp[2].tRelease = value;
            break;
        case 73:
            p->comp[2].makeUpGain = value;
            break;
        case 74:
            p->comp[2].sample_rate = (uint32_t)value;
            break;
        case 75:
            p->comp[2].tEnvAttack = value;
            break;
        case 76:
            p->comp[2].tEnvRelease = value;
            break;
        case 77:
            p->comp[3].thrsh = value;
            break;
        case 78:
            p->comp[3].ratio = value;
            break;
        case 79:
            p->comp[3].tAttack = value;
            break;
        case 80:
            p->comp[3].tRelease = value;
            break;
        case 81:
            p->comp[3].makeUpGain = value;
            break;
        case 82:
            p->comp[3].sample_rate = (uint32_t)value;
            break;
        case 83:
            p->comp[3].tEnvAttack = value;
            break;
        case 84:
            p->comp[3].tEnvRelease = value;
            break;
        case 304:
            p->comp[0].bypass = value;
            break;
        case 305:
            p->comp[1].bypass = value;
            break;
        case 306:
            p->comp[2].bypass = value;
            break;
        case 307:
            p->comp[3].bypass = value;
            break;
        case 303:
            p->bypass = value;
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

    c->bypass = p->bypass;
    for(uint32_t i = 0; i < 4; i++)
    {
        c->comp[i].bypass = p->comp[i].bypass;
        comp_update_coeffs(&p->comp[i], &c->comp[i]);
    }

    return 0;
}




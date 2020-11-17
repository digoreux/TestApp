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

    // comp_control_initialize(&p->comp1_p, &c->comp1_c, sample_rate);
    // comp_control_initialize(&p->comp2_p, &c->comp2_c, sample_rate);
    // comp_control_initialize(&p->comp3_p, &c->comp3_c, sample_rate);
    // comp_control_initialize(&p->comp4_p, &c->comp4_c, sample_rate);
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
            p->comp1_p.thrsh = value;
            break;
        case 54:
            p->comp1_p.ratio = value;
            break;
        case 55:
            p->comp1_p.tAttack = value;
            break;
        case 56:
            p->comp1_p.tRelease = value;
            break;
        case 57:
            p->comp1_p.makeUpGain = value;
            break;
        case 58:
            p->comp1_p.sample_rate = (uint32_t)value;
            break;
        case 59:
            p->comp1_p.tEnvAttack = value;
            break;
        case 60:
            p->comp1_p.tEnvRelease = value;
            break;
        case 61:
            p->comp2_p.thrsh = value;
            break;
        case 62:
            p->comp2_p.ratio = value;
            break;
        case 63:
            p->comp2_p.tAttack = value;
            break;
        case 64:
            p->comp2_p.tRelease = value;
            break;
        case 65:
            p->comp2_p.makeUpGain = value;
            break;
        case 66:
            p->comp2_p.sample_rate = (uint32_t)value;
            break;
        case 67:
            p->comp2_p.tEnvAttack = value;
            break;
        case 68:
            p->comp2_p.tEnvRelease = value;
            break;
        case 69:
            p->comp3_p.thrsh = value;
            break;
        case 70:
            p->comp3_p.ratio = value;
            break;
        case 71:
            p->comp3_p.tAttack = value;
            break;
        case 72:
            p->comp3_p.tRelease = value;
            break;
        case 73:
            p->comp3_p.makeUpGain = value;
            break;
        case 74:
            p->comp3_p.sample_rate = (uint32_t)value;
            break;
        case 75:
            p->comp3_p.tEnvAttack = value;
            break;
        case 76:
            p->comp3_p.tEnvRelease = value;
            break;
        case 77:
            p->comp4_p.thrsh = value;
            break;
        case 78:
            p->comp4_p.ratio = value;
            break;
        case 79:
            p->comp4_p.tAttack = value;
            break;
        case 80:
            p->comp4_p.tRelease = value;
            break;
        case 81:
            p->comp4_p.makeUpGain = value;
            break;
        case 82:
            p->comp4_p.sample_rate = (uint32_t)value;
            break;
        case 83:
            p->comp4_p.tEnvAttack = value;
            break;
        case 84:
            p->comp4_p.tEnvRelease = value;
            break;
        case 304:
            p->comp1_p.bypass = value;
            break;
        case 305:
            p->comp2_p.bypass = value;
            break;
        case 306:
            p->comp3_p.bypass = value;
            break;
        case 307:
            p->comp4_p.bypass = value;
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
    c->comp1_c.bypass = p->comp1_p.bypass;
    c->comp2_c.bypass = p->comp2_p.bypass;
    c->comp3_c.bypass = p->comp3_p.bypass;
    c->comp4_c.bypass = p->comp4_p.bypass;

    comp_update_coeffs(&p->comp1_p, &c->comp1_c);
    comp_update_coeffs(&p->comp2_p, &c->comp2_c);
    comp_update_coeffs(&p->comp3_p, &c->comp3_c);
    comp_update_coeffs(&p->comp4_p, &c->comp4_c);

    return 0;
}




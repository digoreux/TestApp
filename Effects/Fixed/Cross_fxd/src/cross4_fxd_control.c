#include "cross4_fxd_control.h"

int32_t cross4_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes)
{
    *params_bytes = sizeof(cross4_params_t);
    *coeffs_bytes = sizeof(cross4_coeffs_t);
    return 0;
}

int32_t cross4_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate)
{   
    cross4_params_t * p = (cross4_params_t*)params;
    cross4_coeffs_t * c = (cross4_coeffs_t*)coeffs;
    
    return 0;   
}

int32_t cross4_set_parameter(
    void*       params,
    int32_t     id,
    float       value)
{   
    cross4_params_t * p = (cross4_params_t*)params;
    switch (id)
    {
        case 49:
            p->cross1_p.freq = value;
            break;
        case 50:
            p->cross2_p.freq = value;
            break;
        case 51:
            p->cross3_p.freq = value;
            break;
        case 52:
            p->sample_rate = (uint32_t)value;
            break;
        case 302:
            p->bypass = value;
            break;
    }
    p->cross1_p.sample_rate = p->sample_rate;
    p->cross2_p.sample_rate = p->sample_rate;
    p->cross3_p.sample_rate = p->sample_rate;

    return 0;
}

int32_t cross4_update_coeffs(
    void const* params,
    void*       coeffs)
{   
    cross4_params_t * p = (cross4_params_t*)params;
    cross4_coeffs_t * c = (cross4_coeffs_t*)coeffs;

    c->bypass = p->bypass;
    cross_update_coeffs(&p->cross1_p, &c->cross1_c);
    cross_update_coeffs(&p->cross2_p, &c->cross2_c);
    cross_update_coeffs(&p->cross3_p, &c->cross3_c);

    return 0;
}


#include "cross_fxd_control.h"


int32_t cross_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes)
{
    *params_bytes = sizeof(cross_params_t);
    *coeffs_bytes = sizeof(cross_coeffs_t);
    
    return 0;
}

int32_t cross_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate)
{   
    cross_params_t * p = (cross_params_t*)params;
    cross_coeffs_t * c = (cross_coeffs_t*)coeffs;
    p->sample_rate = sample_rate;
    
    return 0;   
}

int32_t cross_set_parameter(
    void*       params,
    int32_t     id,
    float       value)
{   
    cross_params_t * p = (cross_params_t*)params;
    p->freq = 1000;
    return 0;
}

int32_t cross_update_coeffs(
    void const* params,
    void*       coeffs)
{   
    cross_params_t * p = (cross_params_t*)params;
    cross_coeffs_t * c = (cross_coeffs_t*)coeffs;

    double k0, k1, k2, Q = 0.70710678118;
    double fb = p->freq / Q;

    k0  = (tan(M_PI *  p->freq / p->sample_rate) - 1);
    k0 /= (tan(M_PI *  p->freq / p->sample_rate) + 1);

    k2  = (tan(M_PI *  fb / p->sample_rate) - 1);
    k2 /= (tan(M_PI *  fb / p->sample_rate) + 1);

    k1  = -cos(2 * M_PI  * p->freq / p->sample_rate);
    k1 *= (1.0 - k2);

    c->k0 = double2fixed(k0);
    c->k1 = double2fixed(k1);
    c->k2 = double2fixed(-k2);

    // printf("k0: %f\n", c->k0);
    // printf("k1: %f\n", c->k1);
    // printf("k2:  %f\n\n", c->k2);

    return 0;
}


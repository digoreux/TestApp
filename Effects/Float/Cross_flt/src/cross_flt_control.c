#include "cross_flt_control.h"


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

    k1  = (tan(M_PI *  fb / p->sample_rate) - 1);
    k1 /= (tan(M_PI *  fb / p->sample_rate) + 1);

    k2  = -cos(2 * M_PI  * p->freq / p->sample_rate);
    k2 *= (1.0 - k1);

    // printf("k0: %f \n", (float)k0);
    // printf("k1: %f \n", (float)k1);
    // printf("k2: %f \n", (float)k2);

    set_val(&c->k0, (float)k0);
    set_val(&c->k1, (float)k1);
    set_val(&c->k2, (float)k2);
    set_val(&c->k05, 0.5f);

    // printf("k0: %0.30f \n", c->k0.val[3]);
    // printf("k1: %0.30f \n", c->k1.val[3]);
    // printf("k2: %0.30f \n", c->k2.val[3]);

    return 0;
}


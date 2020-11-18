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
    
    return 0;   
}

int32_t cross_set_parameter(
    void*       params,
    int32_t     id,
    float       value)
{   
    cross_params_t * p = (cross_params_t*)params;
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

    // c->k0 = (float)k0;
    // c->k1 = (float)k1;
    // c->k2 = (float)-k2;

    c->mk0.v = _mm_set_ps((float)k0, (float)k0, 0.0f, 0.0f);
    c->mk1.v = _mm_set_ps((float)k1, (float)k1, 0.0f, 0.0f);
    c->mk2.v = _mm_set_ps(-(float)k2, -(float)k2, 0.0f, 0.0f);
    c->m2.v =  _mm_set_ps(0.5f, 0.5f, 0.0f, 0.0f);

    // printf("k0: %f\n", c->k0);
    // printf("k1: %f\n", c->k1);
    // printf("k2:  %f\n\n", c->k2);

    return 0;
}


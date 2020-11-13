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
    p->samplerate = 48000;
    p->freq[0] = 200;
    p->freq[1] = 1000;
    p->freq[2] = 10000;
    return 0;
}

int32_t cross_update_coeffs(
    void const* params,
    void*       coeffs)
{   
    cross_params_t * p = (cross_params_t*)params;
    cross_coeffs_t * c = (cross_coeffs_t*)coeffs;

    double k0, k1, k2, Q = 0.70710678118;
    double fb = p->freq[0] / Q;

    k0  = (tan(M_PI *  p->freq[0] / p->samplerate) - 1);
    k0 /= (tan(M_PI *  p->freq[0] / p->samplerate) + 1);

    k2  = (tan(M_PI *  fb / p->samplerate) - 1);
    k2 /= (tan(M_PI *  fb / p->samplerate) + 1);

    k1  = -cos(2 * M_PI  * 200 / p->samplerate);
    k1 *= (1.0 - k2);

    c->k0 = (float)k0;
    c->k1 = (float)k1;
    c->k2 = (float)-k2;

    printf("k0: %f\n", c->k0);
    printf("k1: %f\n", c->k1);
    printf("k2:  %f\n", c->k2);

    return 0;
}


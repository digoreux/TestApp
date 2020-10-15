#include "effect_control.h" 
#include "fractional.h"

#define M_PI  3.14159265358979323846

typedef struct coeffs_s {
    flt  b0;
    flt  b1;
    flt  b2;
    flt  a0;
    flt  a1;
    flt  a2;
} coeffs_t;

typedef struct params_s {
    double freq;   // id:0
    double gain;   // id:1
    double  Q;     // id:2
    double SR;     // id:3
}params_t;

int32_t effect_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes)

{   
    *params_bytes = sizeof(params_t);
    *coeffs_bytes = sizeof(coeffs_t);
    return 0;
}

int32_t effect_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate)

{   
    coeffs_t * c = (coeffs_t*)coeffs;
    params_t * p = (params_t*)params;

    p->freq = 0;
    p->gain = 1;
    p->Q = 0;
    p->SR = 48000;

    c->b0 = 0;
    c->b1 = 0;
    c->b2 = 0;
    c->a0 = 0;
    c->a1 = 0;
    c->a2 = 0;

    return 0;
} 

int32_t effect_set_parameter(
    void*       params,
    int32_t     id,
    float       value)
{   
    params_t * p = (params_t*)params;
    if (id == 0) p->freq = value;
    if (id == 1) p->gain = value;
    if (id == 2) p->Q  = value;
    if (id == 3) p->SR = value;
    return 0;
}

int32_t effect_update_coeffs(
    void const* params,
    void*       coeffs)
{   
    coeffs_t * c = (coeffs_t*)coeffs;
    params_t * p = (params_t*)params;

    double A = pow(10, p->gain / 40);
    double omega = 2 * M_PI * p->freq / p->SR;
    double sn = sin(omega);
    double cs = cos(omega);
    double alpha = sn / (2 * p->Q);

    double b0 = (1.0 - cs) / 2.0;
    double b1 =  1.0 - cs;
    double b2 = (1.0 - cs) / 2.0;
    double a0 =  1.0 + alpha;
    double a1 = -2.0 * cs;
    double a2 =  1.0 - alpha;

    c->a1 = (float)(a1 / a0);
    c->a2 = (float)(a2 / a0);
    c->b0 = (float)(b0 / a0);
    c->b1 = (float)(b1 / a0);
    c->b2 = (float)(b2 / a0);

    //printf("a1: %f\n", c->a1);
    //printf("a2: %f\n", c->a2);
    //printf("b0: %f\n", c->b0);
    //printf("b1: %f\n", c->b1);
    //printf("b2: %f\n", c->b2);

    return 0;
}



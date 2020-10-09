#include "effect_control.h" 

#define M_PI  3.14159265358979323846

typedef struct coeffs_s {
    double  b0;
    double  b1;
    double  b2;
    double  a0;
    double  a1;
    double  a2;
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

    c->b0 = (1.0 - cs) / 2.0;
    c->b1 =  1.0 - cs;
    c->b2 = (1.0 - cs) / 2.0;
    c->a0 =  1.0 + alpha;
    c->a1 = -2.0 * cs;
    c->a2 =  1.0 - alpha;

    c->a1 /= c->a0;
    c->a2 /= c->a0;
    c->b0 /= c->a0;
    c->b1 /= c->a0;
    c->b2 /= c->a0;

    return 0;
}



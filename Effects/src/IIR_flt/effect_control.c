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

    double A = fpow(10, divf(p->gain, 40));
    double omega = divf(mulf(mulf(2, M_PI), p->freq), p->SR);
    double sn = sin(omega);
    double cs = cos(omega);
    double alpha = divf(sn, mulf(2, p->Q));


    c->b0 = divf(subf(1.0, cs), 2.0);
    c->b1 = subf(1.0, cs);
    c->b2 = divf(subf(1.0, cs), 2.0);
    c->a0 = addf(1.0, alpha);
    c->a1 = mulf(negf(2.0), cs);
    c->a2 = subf(1.0, alpha);

    c->a1 = divf(c->a1, c->a0);
    c->a2 = divf(c->a2, c->a0);
    c->b0 = divf(c->b0, c->a0);
    c->b1 = divf(c->b1, c->a0);
    c->b2 = divf(c->b2, c->a0);


    return 0;
}



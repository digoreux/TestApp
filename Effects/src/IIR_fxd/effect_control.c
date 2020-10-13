#include "effect_control.h" 
#include "fractional.h"

#define M_PI  3.14159265358979323846

typedef struct coeffs_s {
    q31  b0;
    q31  b1;
    q31  b2;
    q31  a0;
    q31  a1;
    q31  a2;
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


    double b0 = divf(subf(1.0, cs), 2.0);
    double b1 = subf(1.0, cs);
    double b2 = divf(subf(1.0, cs), 2.0);
    double a0 = addf(1.0, alpha);
    double a1 = mulf(negf(2.0), cs);
    double a2 = subf(1.0, alpha);


    a1 = divf(a1, a0);
    a2 = divf(a2, a0);
    b0 = divf(b0, a0);
    b1 = divf(b1, a0);
    b2 = divf(b2, a0);


    //printf("a0 %lf\n", a0);
    //printf("a1 %lf\n", a1);
    //printf("a2 %lf\n", a2);
    //printf("b0 %lf\n", b0);
    //printf("b1 %lf\n", b1);
    //printf("b2 %lf\n\n", b2);

    c->a1 = double2fixed(a1); 
    c->a2 = double2fixed(a2); 
    c->b0 = double2fixed(b0); 
    c->b1 = double2fixed(b1); 
    c->b2 = double2fixed(b2); 
    
    //c->a1 <<= 1;
    //c->a2 <<= 1;
    //c->b0 <<= 1;
    //c->b1 <<= 1;
    //c->b2 <<= 1;

    //printf("a1 %ld\n", c->a1);
    //printf("a2 %ld\n", c->a2);
    //printf("b0 %ld\n", c->b0);
    //printf("b1 %ld\n", c->b1);
    //printf("b2 %ld\n\n", c->b2); 
    //
    //printf("a1 %lf\n", fixed2double(c->a1));
    //printf("a2 %lf\n", fixed2double(c->a2));
    //printf("b0 %lf\n", fixed2double(c->b0));
    //printf("b1 %lf\n", fixed2double(c->b1));
    //printf("b2 %lf\n", fixed2double(c->b2));

    return 0;
}



#include "effect_control.h" 
#include "fractional.h"

#define M_PI  3.14159265358979323846

typedef enum {
	LP   = 0,
	HP   = 1,
	PEAK = 2,
	LSH  = 3,
	HSH	 = 4,
    OFF  = 5
} filter_types;

typedef struct param_s {
    int32_t id;
    double  value;
} param_t;


typedef struct params_s {
    double sample_rate;
    param_t  freq[10]; 
    param_t  gain[10]; 
    param_t     Q[10];   
    param_t  type[10];
} params_t;


typedef struct coeffs_s {
    flt  b0[10];
    flt  b1[10];
    flt  b2[10];
    flt  a0[10];
    flt  a1[10];
    flt  a2[10];
} coeffs_t;



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

    p->sample_rate = sample_rate;
    for (size_t i = 0; i < 10; i++)
    {
        p->freq[i].id = 0 + i*4;
        p->gain[i].id = 1 + i*4;
        p->Q[i].id    = 2 + i*4;
        p->type[i].id = 3 + i*4;

        p->freq[i].value = 0;
        p->gain[i].value = 0;
        p->Q[i].value    = 0;
        p->type[i].value = 0;

        c->b0[i] = 0;
        c->b1[i] = 0;
        c->b2[i] = 0;
        c->a0[i] = 0;
        c->a1[i] = 0;
        c->a2[i] = 0;
    }
    return 0;
} 

int32_t effect_set_parameter(
    void*       params,
    int32_t     id,
    float       value)
{   
    params_t * p = (params_t*)params;
    if(id == 40) p->sample_rate = value;
    for(int i = 0; i < 10; i++) 
    {
        if(p->freq[i].id == id) p->freq[i].value = value;
        if(p->gain[i].id == id) p->gain[i].value = value;
        if(p->Q[i].id    == id)    p->Q[i].value = value;
        if(p->type[i].id == id) p->type[i].value = value;
    }           
    
    return 0;
}

int32_t effect_update_coeffs(
    void const* params,
    void*       coeffs)
{   
    double  A[10], sn[10], cs[10], alpha[10], beta[10], omega[10];
    double b0[10], b1[10], b2[10], a0[10], a1[10], a2[10];

    coeffs_t * c = (coeffs_t*)coeffs;
    params_t * p = (params_t*)params;

    for (size_t i = 0; i < 10; i++) 
    {
        A[i] = pow(10, p->gain[i].value / 40);
        omega[i] = 2 * M_PI * p->freq[i].value / p->sample_rate;
        sn[i] = sin(omega[i]);
        cs[i] = cos(omega[i]);
        alpha[i] = sn[i] / (2 * p->Q[i].value);
        beta[i]  = sqrt(A[i] + A[i]);
        switch ((int)p->type[i].value)
        {
        case LP:
            b0[i] = (1.0 - cs[i]) / 2.0;
            b1[i] =  1.0 - cs[i];
            b2[i] = (1.0 - cs[i]) / 2.0;
            a0[i] =  1.0 + alpha[i];
            a1[i] = -2.0 * cs[i];
            a2[i] =  1.0 - alpha[i];
            break;
        case HP:
            b0[i] =  (1 + cs[i]) /2.0;
            b1[i] = -(1 + cs[i]);
            b2[i] =  (1 + cs[i]) /2.0;
            a0[i] =  1 + alpha[i];
            a1[i] = -2 * cs[i];
            a2[i] =  1 - alpha[i];
            break;
        case PEAK:
            b0[i] =  1 + (alpha[i] * A[i]);
            b1[i] = -2 * cs[i];
            b2[i] =  1 - (alpha[i] * A[i]);
            a0[i] =  1 + (alpha[i] / A[i]);
            a1[i] = -2 * cs[i];
            a2[i] =  1 - (alpha[i] / A[i]);
            break;
        case LSH:
            b0[i] = A[i] * ((A[i] + 1) - (A[i] - 1) * cs[i] + beta[i] * sn[i]);
    	    b1[i] = A[i] * ((A[i] - 1) - (A[i] + 1) * cs[i]) * 2;
    	    b2[i] = A[i] * ((A[i] + 1) - (A[i] - 1) * cs[i] - beta[i] * sn[i]); 
    	    a0[i] =  (A[i] + 1) + (A[i] - 1) * cs[i] + beta[i] * sn[i];
    	    a1[i] = ((A[i] - 1) + (A[i] + 1) * cs[i]) * (-2);
    	    a2[i] =  (A[i] + 1) + (A[i] - 1) * cs[i] - beta[i] * sn[i];
            break;
        case HSH:
            b0[i] = A[i] * ((A[i] + 1) + (A[i] - 1) * cs[i] + beta[i] * sn[i]);
            b1[i] = A[i] * ((A[i] - 1) + (A[i] + 1) * cs[i]) * (-2);
            b2[i] = A[i] * ((A[i] + 1) + (A[i] - 1) * cs[i] - beta[i] * sn[i]);
            a0[i] =  (A[i] + 1) - (A[i] - 1) * cs[i] + beta[i] * sn[i];
            a1[i] = ((A[i] - 1) - (A[i] + 1) * cs[i]) * 2;
            a2[i] =  (A[i] + 1) - (A[i] - 1) * cs[i] - beta[i] * sn[i];
            break;
        case OFF:
            b0[i] = 0;
            b1[i] = 0;
            b2[i] = 0;
            a0[i] = 0;
            a1[i] = 0;
            a2[i] = 0;
            break;
        }

        // a0[i] /= a0[i];
        b0[i] /= a0[i];
        b1[i] /= a0[i];
        b2[i] /= a0[i];
        a1[i] /= a0[i];
        a2[i] /= a0[i];



         c->a0[i] = (float)(a0[i] / 8);
         c->a1[i] = (float)(a1[i] / 8);
         c->a2[i] = (float)(a2[i] / 8);
         c->b0[i] = (float)(b0[i] / 8);
         c->b1[i] = (float)(b1[i] / 8);
         c->b2[i] = (float)(b2[i] / 8);

        //c->a0[i] = (float)(a0[i]);
        //c->a1[i] = (float)(a1[i]);
        //c->a2[i] = (float)(a2[i]);
        //c->b0[i] = (float)(b0[i]);
        //c->b1[i] = (float)(b1[i]);
        //c->b2[i] = (float)(b2[i]);

        // printf("a0: %f \n", c->a0[i]);
        // printf("a1: %f \n", c->a1[i]);
        // printf("a2: %f \n", c->a2[i]);
        // printf("b0: %f \n", c->b0[i]);
        // printf("b1: %f \n", c->b1[i]);
        // printf("b2: %f \n", c->b2[i]);
        // printf("\n");


    }

    printf("a0: %f \n", c->a0[0]);
    printf("a1: %f \n", c->a1[0]);
    printf("a2: %f \n", c->a2[0]);
    printf("b0: %f \n", c->b0[0]);
    printf("b1: %f \n", c->b1[0]);
    printf("b2: %f \n", c->b2[0]);

    return 0;
}



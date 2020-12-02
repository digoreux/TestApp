#include "biquad_fxd_control.h" 


int32_t bq_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes)

{   
    *params_bytes = sizeof(bq_params_t);
    *coeffs_bytes = sizeof(bq_coeffs_t);
    return 0;
}

int32_t bq_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate)

{   
    bq_coeffs_t * c = (bq_coeffs_t*)coeffs;
    bq_params_t * p = (bq_params_t*)params;
    p->sample_rate = sample_rate;

        p->freq = 0;
        p->gain = 0;
        p->Q    = 0;
        p->type = 5;
        p->bypass = false;
        
        c->b0 = 0;
        c->b1 = 0;
        c->b2 = 0;
        c->a0 = 0;
        c->a1 = 0;
        c->a2 = 0;

    return 0;
} 

int32_t bq_set_parameter(
    void*       params,
    int32_t     id,
    float       value)
{   
    bq_params_t * p = (bq_params_t*)params;
    
    // p->freq = 1000;
    // p->gain = -6;
    // p->Q    = 6;
    // p->type = 2;
    p->freq = 1000;
    p->gain = 0;
    p->Q    = 0.5;
    p->type = 0;
    p->sample_rate = 48000;
    
    return 0;
}

int32_t bq_update_coeffs(
    void const* params,
    void*       coeffs)
{   
    double  A, sn, cs, alpha, beta, omega;
    double b0, b1, b2, a0, a1, a2;

    bq_coeffs_t * c = (bq_coeffs_t*)coeffs;
    bq_params_t * p = (bq_params_t*)params;

    c->bypass = p->bypass;
    A = pow(10, p->gain / 40);
    omega = 2 * M_PI * p->freq / p->sample_rate;
    sn = sin(omega);
    cs = cos(omega);
    alpha = sn / (2 * p->Q);
    beta  = sqrt(A + A);

    switch ((int)p->type)
    {
    case LP:
        b0 = (1.0 - cs) / 2.0;
        b1 =  1.0 - cs;
        b2 = (1.0 - cs) / 2.0;
        a0 =  1.0 + alpha;
        a1 = -2.0 * cs;
        a2 =  1.0 - alpha;
        break;
    case HP:
        b0 =  (1 + cs) /2.0;
        b1 = -(1 + cs);
        b2 =  (1 + cs) /2.0;
        a0 =  1 + alpha;
        a1 = -2 * cs;
        a2 =  1 - alpha;
        break;
    case PEAK:
        b0 =  1 + (alpha * A);
        b1 = -2 * cs;
        b2 =  1 - (alpha * A);
        a0 =  1 + (alpha / A);
        a1 = -2 * cs;
        a2 =  1 - (alpha / A);
        break;
    case LSH:
        b0 = A * ((A + 1) - (A - 1) * cs + beta * sn);
	    b1 = A * ((A - 1) - (A + 1) * cs) * 2;
	    b2 = A * ((A + 1) - (A - 1) * cs - beta * sn); 
	    a0 =  (A + 1) + (A - 1) * cs + beta * sn;
	    a1 = ((A - 1) + (A + 1) * cs) * (-2);
	    a2 =  (A + 1) + (A - 1) * cs - beta * sn;
        break;
    case HSH:
        b0 = A * ((A + 1) + (A - 1) * cs + beta * sn);
        b1 = A * ((A - 1) + (A + 1) * cs) * (-2);
        b2 = A * ((A + 1) + (A - 1) * cs - beta * sn);
        a0 =  (A + 1) - (A - 1) * cs + beta * sn;
        a1 = ((A - 1) - (A + 1) * cs) * 2;
        a2 =  (A + 1) - (A - 1) * cs - beta * sn;
        break;
    case OFF:
        b0 = 0;
        b1 = 0;
        b2 = 0;
        a0 = 0;
        a1 = 0;
        a2 = 0;
        break;
    }
    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= a0;
    a2 /= a0;
    
    // printf("b0: %0.20f \n", (float)b0);
    // printf("b1: %0.20f \n", (float)b1);
    // printf("b2: %0.20f \n", (float)b2);
    // printf("a1: %0.20f \n", (float)a1);
    // printf("a2: %0.20f \n", (float)a2);

    c->a0 = double2fixed_q((a0 / 8), 31);
    c->a1 = double2fixed_q((a1 / 8), 31);
    c->a2 = double2fixed_q((a2 / 8), 31);
    c->b0 = double2fixed_q((b0 / 8), 31);
    c->b1 = double2fixed_q((b1 / 8), 31);
    c->b2 = double2fixed_q((b2 / 8), 31);


    return 0;
}



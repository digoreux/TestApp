#include "biquad_flt_control.h" 


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

    return 0;
} 

int32_t bq_set_parameter(
    void*       params,
    int32_t     id,
    float       value)
{   
    bq_params_t * p = (bq_params_t*)params;
    
    p->freq = 20.0f;
    p->gain = -6.0f;
    p->Q    = 6.0f;
    p->type = 2;
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
    A = pow(10.0, p->gain / 40.0);
    omega = 2.0 * M_PI * p->freq / p->sample_rate;
    sn = sin(omega);
    cs = cos(omega);
    alpha = sn / (2.0 * p->Q);
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
        b0 =  (1.0 + cs) /2.0;
        b1 = -(1.0 + cs);
        b2 =  (1.0 + cs) /2.0;
        a0 =  1.0 + alpha;
        a1 = -2.0 * cs;
        a2 =  1.0 - alpha;
        break;
    case PEAK:                      
        b0 =  1.0 + (alpha * A);
        b1 = -2.0 * cs;
        b2 =  1.0 - (alpha * A);
        a0 =  1.0 + (alpha / A);
        a1 = -2.0 * cs;
        a2 =  1.0 - (alpha / A);
        break;
    case LSH:
        b0 = A * ((A + 1.0) - (A - 1.0) * cs + beta * sn);
	    b1 = A * ((A - 1.0) - (A + 1.0) * cs) * 2.0;
	    b2 = A * ((A + 1.0) - (A - 1.0) * cs - beta * sn); 
	    a0 =  (A + 1.0) + (A - 1.0) * cs + beta * sn;
	    a1 = ((A - 1.0) + (A + 1.0) * cs) * (-2.0);
	    a2 =  (A + 1.0) + (A - 1.0) * cs - beta * sn;
        break;
    case HSH:
        b0 = A * ((A + 1.0) + (A - 1.0) * cs + beta * sn);
        b1 = A * ((A - 1.0) + (A + 1.0) * cs) * (-2.0);
        b2 = A * ((A + 1.0) + (A - 1.0) * cs - beta * sn);
        a0 =  (A + 1.0) - (A - 1.0) * cs + beta * sn;
        a1 = ((A - 1.0) - (A + 1.0) * cs) * 2.0;
        a2 =  (A + 1.0) - (A - 1.0) * cs - beta * sn;
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
    a1 /= -a0;
    a2 /= -a0;

    set_val(&c->a0, (float)a0);
    set_val(&c->a1, (float)a1);
    set_val(&c->a2, (float)a2);
    set_val(&c->b0, (float)b0);
    set_val(&c->b1, (float)b1);
    set_val(&c->b2, (float)b2);

    return 0;
}
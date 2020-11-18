#include "eq_flt_control.h" 

#define M_PI  3.14159265358979323846

int32_t eq_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes)

{   
    *params_bytes = sizeof(eq_params_t);
    *coeffs_bytes = sizeof(eq_coeffs_t);
    return 0;
}

int32_t eq_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate)

{   
    eq_coeffs_t * c = (eq_coeffs_t*)coeffs;
    eq_params_t * p = (eq_params_t*)params;
    c->bypass = p->bypass;
    p->sample_rate = sample_rate;
    for (uint32_t i = 0; i < 10; i++)
    {
        p->freq[i].id = 0 + i*4;
        p->gain[i].id = 1 + i*4;
        p->Q[i].id    = 2 + i*4;
        p->type[i].id = 3 + i*4;

        p->freq[i].value = 0;
        p->gain[i].value = 0;
        p->Q[i].value    = 0;
        p->type[i].value = 5;

        for (uint32_t j = 0; j < 6; j++)
            c->k[j][i] = 0;
    }
    return 0;
} 

int32_t eq_set_parameter(
    void*       params,
    int32_t     id,
    float       value)
{   
    eq_params_t * p = (eq_params_t*)params;
    if(id == 40) p->sample_rate = (uint32_t)value;
    if(id == 300) p->bypass = value;
    for(int i = 0; i < 10; i++) 
    {
        if(p->freq[i].id == id) p->freq[i].value = value;
        if(p->gain[i].id == id) p->gain[i].value = value;
        if(p->Q[i].id    == id)    p->Q[i].value = value;
        if(p->type[i].id == id) p->type[i].value = value;
    }           
    
    return 0;
}

int32_t eq_update_coeffs(
    void const* params,
    void*       coeffs)
{   
    double  A[10], sn[10], cs[10], alpha[10], beta[10], omega[10];
    double b0[10], b1[10], b2[10], a0[10], a1[10], a2[10];

    eq_coeffs_t * c = (eq_coeffs_t*)coeffs;
    eq_params_t * p = (eq_params_t*)params;
    c->bypass = p->bypass;
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

        b0[i] /= a0[i];
        b1[i] /= a0[i];
        b2[i] /= a0[i];
        a1[i] /= -a0[i];
        a2[i] /= -a0[i];

         c->mk[0][i].v = _mm_set_ps((float)a0[i], (float)a0[i], 0.0f, 0.0f);
         c->mk[1][i].v = _mm_set_ps((float)a1[i], (float)a1[i], 0.0f, 0.0f);
         c->mk[2][i].v = _mm_set_ps((float)a2[i], (float)a2[i], 0.0f, 0.0f);
         c->mk[3][i].v = _mm_set_ps((float)b0[i], (float)b0[i], 0.0f, 0.0f);
         c->mk[4][i].v = _mm_set_ps((float)b1[i], (float)b1[i], 0.0f, 0.0f);
         c->mk[5][i].v = _mm_set_ps((float)b2[i], (float)b2[i], 0.0f, 0.0f);

        c->k[0][i] = (float)a0[i];
        c->k[1][i] = (float)a1[i];
        c->k[2][i] = (float)a2[i];
        c->k[3][i] = (float)b0[i];
        c->k[4][i] = (float)b1[i];
        c->k[5][i] = (float)b2[i];
 /*       printf("k0: %f \n", c->k[0][i]);
        printf("k1: %f \n", c->k[1][i]);
        printf("k2: %f \n", c->k[2][i]);
        printf("k3: %f \n", c->k[3][i]);
        printf("k4: %f \n", c->k[4][i]);
        printf("k5: %f \n", c->k[5][i]);
        printf("\n");*/
        // printf("a0: %f \n",  c->mk[0][i].f[3]);
        // printf("a1: %f \n",  c->mk[1][i].f[3]);
        // printf("a2: %f \n",  c->mk[2][i].f[3]);
        // printf("b0: %f \n",  c->mk[3][i].f[3]);
        // printf("b1: %f \n",  c->mk[4][i].f[3]);
        // printf("b2: %f \n",  c->mk[5][i].f[3]);
    }


    return 0;
}



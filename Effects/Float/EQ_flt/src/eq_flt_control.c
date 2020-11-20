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

    c->bypass = false;
    p->sample_rate = sample_rate;

    for (uint32_t i = 0; i < 10; i++) 
    {
        bq_control_initialize(&p->bq[i], &c->bq[i], sample_rate);
        // eq_update_coeffs(&p->bq[i], &c->bq[i]);
    }

    return 0;
} 

int32_t eq_set_parameter(
    void*       params,
    int32_t     id,
    float       value)
{   
    eq_params_t * p = (eq_params_t*)params;
    switch(id)
    {
        case 0: 
            p->bq[0].freq = value;
            break;
        case 1: 
            p->bq[0].gain = value;
            break;
        case 2: 
            p->bq[0].Q = value;
            break;
        case 3: 
            p->bq[0].type = value;
            break;
        case 4: 
            p->bq[1].freq = value;
            break;
        case 5: 
            p->bq[1].gain = value;
            break;
        case 6: 
            p->bq[1].Q = value;
            break;
        case 7: 
            p->bq[1].type = value;
            break;
        case 8: 
            p->bq[2].freq = value;
            break;
        case 9: 
            p->bq[2].gain = value;
            break;
        case 10: 
            p->bq[2].Q = value;
            break;
        case 11: 
            p->bq[2].type = value;
            break;
        case 12: 
            p->bq[3].freq = value;
            break;
        case 13: 
            p->bq[3].gain = value;
            break;
        case 14: 
            p->bq[3].Q = value;
            break;
        case 15: 
            p->bq[3].type = value;
            break;
        case 16: 
            p->bq[4].freq = value;
            break;
        case 17: 
            p->bq[4].gain = value;
            break;
        case 18: 
            p->bq[4].Q = value;
            break;
        case 19: 
            p->bq[4].type = value;
            break;
        case 20: 
            p->bq[5].freq = value;
            break;
        case 21: 
            p->bq[5].gain = value;
            break;
        case 22: 
            p->bq[5].Q = value;
            break;
        case 23: 
            p->bq[5].type = value;
            break;
        case 24: 
            p->bq[6].freq = value;
            break;
        case 25: 
            p->bq[6].gain = value;
            break;
        case 26: 
            p->bq[6].Q = value;
            break;
        case 27: 
            p->bq[6].type = value;
            break;
        case 28: 
            p->bq[7].freq = value;
            break;
        case 29: 
            p->bq[7].gain = value;
            break;
        case 30: 
            p->bq[7].Q = value;
            break;
        case 31: 
            p->bq[7].type = value;
            break;
        case 32: 
            p->bq[8].freq = value;
            break;
        case 33: 
            p->bq[8].gain = value;
            break;
        case 34: 
            p->bq[8].Q = value;
            break;
        case 35: 
            p->bq[8].type = value;
            break;
        case 36: 
            p->bq[9].freq = value;
            break;
        case 37: 
            p->bq[9].gain = value;
            break;
        case 38: 
            p->bq[9].Q = value;
            break;
        case 39: 
            p->bq[9].type = value;
            break;
        case 40: 
            p->sample_rate = (uint32_t)value;
            break;
        case 300: 
            p->bypass = value;
            break;

    }
         
    return 0;
}

int32_t eq_update_coeffs(
    void const* params,
    void*       coeffs)
{   
    eq_coeffs_t * c = (eq_coeffs_t*)coeffs;
    eq_params_t * p = (eq_params_t*)params;

    c->bypass = p->bypass;

    for (size_t i = 0; i < 10; i++) 
    {
        bq_update_coeffs(&p->bq[i], &c->bq[i]);

        // printf("a0[%d]: %f \n", i, c->bq[i].a0.val[3]);
        // printf("a1[%d]: %f \n", i, c->bq[i].a1.val[3]);
        // printf("a2[%d]: %f \n", i, c->bq[i].a2.val[3]);
        // printf("b0[%d]: %f \n", i, c->bq[i].b0.val[3]);
        // printf("b1[%d]: %f \n", i, c->bq[i].b1.val[3]);
        // printf("b2[%d]: %f \n", i, c->bq[i].b2.val[3]);
        // printf("\n");
    }

    return 0;
}



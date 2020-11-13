#include "chain_flt_control.h"

#include "cross_flt_control.h"
#include "comp_flt_control.h"
#include "eq_flt_control.h"

typedef struct params_t {
    eq_params_t   eq_p;
    comp_params_t comp_p;
    cross_params_t cross_p;
} params_t;

typedef struct coeffs_t {
    eq_coeffs_t   eq_c;
    comp_coeffs_t comp_c;
    cross_coeffs_t cross_c;
} coeffs_t;

typedef struct states_t {
    eq_states_t   eq_s;
    comp_states_t comp_s;
    cross_states_t cross_s;
} states_t;


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
    params_t *p = (params_t*)params;
    coeffs_t *c = (coeffs_t*)coeffs;

    eq_control_initialize(&p->eq_p, &c->eq_c, p->eq_p.sample_rate);
    comp_control_initialize(&p->comp_p, &c->comp_c, p->comp_p.samplerate);
    cross_control_initialize(&p->cross_p, &c->cross_c, p->cross_p.samplerate);


    return 0;
}

int32_t effect_set_parameter(
    void*       params,
    int32_t     id,
    float       value)                    
{   
    params_t *p = (params_t*)params; 
    
    eq_set_parameter(&p->eq_p, id, value);
    comp_set_parameter(&p->comp_p, id, value);
    cross_set_parameter(&p->cross_p, id, value);

    return 0;
}

int32_t effect_update_coeffs(
    void const* params,
    void*       coeffs)
{   
    params_t *p = (params_t*)params;
    coeffs_t *c = (coeffs_t*)coeffs;

    eq_update_coeffs(&p->eq_p, &c->eq_c);   
    comp_update_coeffs(&p->comp_p, &c->comp_c);   
    cross_update_coeffs(&p->cross_p, &c->cross_c);

    return 0;
}



#include "effect_control.h"
#include "effect_structure.h"

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
    p->bq.sample_rate = sample_rate;
    p->eq.sample_rate = sample_rate;
    p->comp.sample_rate = sample_rate;
    p->cross.sample_rate = sample_rate;
    p->comp4.sample_rate = sample_rate;
    p->comp4s.sample_rate = sample_rate;
    p->cross4.sample_rate = sample_rate;
    eq_control_initialize(&p->eq, &c->eq, p->eq.sample_rate);
    bq_control_initialize(&p->bq, &c->bq, p->bq.sample_rate);
    comp_control_initialize(&p->comp, &c->comp, p->comp.sample_rate);
    cross_control_initialize(&p->cross, &c->cross, p->cross.sample_rate);
    comp4_control_initialize(&p->comp4, &c->comp4, p->comp4.sample_rate);
    comp4s_control_initialize(&p->comp4s, &c->comp4s, p->comp4.sample_rate);
    cross4_control_initialize(&p->cross4, &c->cross4, p->cross4.sample_rate);


    return 0;
}

int32_t effect_set_parameter(
    void*       params,
    int32_t     id,
    float       value)                    
{   
    params_t *p = (params_t*)params; 
    
    eq_set_parameter(&p->eq, id, value);
    bq_set_parameter(&p->bq, id, value);
    comp_set_parameter(&p->comp, id, value);
    cross_set_parameter(&p->cross, id, value);
    comp4_set_parameter(&p->comp4, id, value);
    comp4s_set_parameter(&p->comp4s, id, value);
    cross4_set_parameter(&p->cross4, id, value);

    return 0;
}

int32_t effect_update_coeffs(
    void const* params,
    void*       coeffs)
{   
    params_t *p = (params_t*)params;
    coeffs_t *c = (coeffs_t*)coeffs;

    bq_update_coeffs(&p->bq, &c->bq);
    eq_update_coeffs(&p->eq, &c->eq);  
    comp_update_coeffs(&p->comp, &c->comp);   
    cross_update_coeffs(&p->cross, &c->cross);
    comp4_update_coeffs(&p->comp4, &c->comp4);
    comp4s_update_coeffs(&p->comp4s, &c->comp4s);
    cross4_update_coeffs(&p->cross4, &c->cross4);

    return 0;
}



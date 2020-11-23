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
    p->bq_p.sample_rate = sample_rate;
    p->eq_p.sample_rate = sample_rate;
    p->comp_p.sample_rate = sample_rate;
    p->cross_p.sample_rate = sample_rate;
    p->comp4_p.sample_rate = sample_rate;
    p->expand_p.sample_rate = sample_rate;
    p->cross4_p.sample_rate = sample_rate;
    eq_control_initialize(&p->eq_p, &c->eq_c, p->eq_p.sample_rate);
    bq_control_initialize(&p->bq_p, &c->bq_c, p->bq_p.sample_rate);
    cross_control_initialize(&p->cross_p, &c->cross_c, p->cross_p.sample_rate);
    // comp_control_initialize(&p->comp_p, &c->comp_c, p->comp_p.sample_rate);
    // comp4_control_initialize(&p->comp4_p, &c->cross4_c, p->comp4_p.sample_rate);
    // expand_control_initialize(&p->expand_p, &c->expand_c, p->expand_p.sample_rate);
    // cross4_control_initialize(&p->cross4_p, &c->cross4_c, p->cross4_p.sample_rate);

    return 0;
}

int32_t effect_set_parameter(
    void*       params,
    int32_t     id,
    float       value)                    
{   
    params_t *p = (params_t*)params; 
    
    eq_set_parameter(&p->eq_p, id, value);
    bq_set_parameter(&p->bq_p, id, value);
    cross_set_parameter(&p->cross_p, id, value);
    // comp_set_parameter(&p->comp_p, id, value);
    // comp4_set_parameter(&p->comp4_p, id, value);
    // expand_set_parameter(&p->expand_p, id, value);
    // cross4_set_parameter(&p->cross4_p, id, value);

    return 0;
}

int32_t effect_update_coeffs(
    void const* params,
    void*       coeffs)
{   
    params_t *p = (params_t*)params;
    coeffs_t *c = (coeffs_t*)coeffs;

    bq_update_coeffs(&p->bq_p, &c->bq_c);
    eq_update_coeffs(&p->eq_p, &c->eq_c);  
    cross_update_coeffs(&p->cross_p, &c->cross_c);
    // comp_update_coeffs(&p->comp_p, &c->comp_c);   
    // comp4_update_coeffs(&p->comp4_p, &c->comp4_c);
    // expand_update_coeffs(&p->expand_p, &c->expand_c);   
    // cross4_update_coeffs(&p->cross4_p, &c->cross4_c);

    return 0;
}



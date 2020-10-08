#include "effect_control.h" 



int32_t effect_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes)

{   
    return 0;
}

int32_t effect_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate)

{   
    coeffs_t * coef = (coeffs_t*)coeffs;
    coef->a1 = 0.9510565162951534;
    coef->a2 = -0.24999999999999992;
    coef->b0 = 0.07473587092621162;
    coef->b1 = 0.14947174185242323;
    coef->b2 = 0.07473587092621162;

    return 0;
} 

int32_t effect_set_parameter(
    void*       params,
    int32_t     id,
    float       value)
{
    return 0;
}

int32_t effect_update_coeffs(
    void const* params,
    void*       coeffs)
{
    return 0;
}



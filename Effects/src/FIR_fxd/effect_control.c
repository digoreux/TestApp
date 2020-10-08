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

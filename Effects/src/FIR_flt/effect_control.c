#include "effect_control.h" 

#define NTAPS 128

extern double fir_flt_coeffs[128];

int32_t effect_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes)

{   
    *coeffs_bytes = sizeof(fir_flt_coeffs);
    return 0;
}

int32_t effect_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate)

{   
    memcpy(coeffs, fir_flt_coeffs, sizeof(fir_flt_coeffs));

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



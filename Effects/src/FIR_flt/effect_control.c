#include "effect_control.h" 


extern float fir_flt_coeffs[128];

int32_t effect_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes)

{   
    params_bytes = 0;
    coeffs_bytes = sizeof(float) * 128;
    return 0;
}

int32_t effect_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate)

{   
  
    memcpy(coeffs, fir_flt_coeffs, sizeof(float) * 128);
    params = NULL;

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



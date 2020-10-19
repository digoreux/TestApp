#include "effect_control.h"

typedef struct params_s {
    float threshold;
    float ratio;
    float atack;
    float release;
} params_t;

typedef struct coeffs_t {
    float gain;
    float ratio;
} coeffs_t;

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



#include "effect_control.h"
#include "comp_effect_control.h"
#include "eq_effect_control.h"
typedef struct param_s {
    int32_t id;
    double  value;
} param_t;


typedef struct params_s {
    eq_params_t 
    comp_params_t 
} params_t


int32_t effect_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes)
{
    *params_bytes = 4;
    *coeffs_bytes = 4;

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
    *(float*)params = value;
    return 0;
}

int32_t effect_update_coeffs(
    void const* params,
    void*       coeffs)
{   
    *(float*) coeffs = (float)pow(10.0, *(float*)params / 20.0);    
    return 0;
}



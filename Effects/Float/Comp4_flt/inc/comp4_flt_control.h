#ifndef __COMP4_FLT_CONTROL_H__
#define __COMP4_FLT_CONTROL_H__
#include "abstract_effect.h"

typedef struct comp4_params_s {
    flt thrsh[4];
    flt ratio[4];
    flt tAttack[4];         // gain
    flt tRelease[4];
    flt tEnvAttack[4];      //env
    flt tEnvRelease[4];
    flt makeUpGain[4];
    uint32_t sample_rate;
    bool downward[4];
    bool bypass[5];
} comp4_params_t;

typedef struct comp4_coeffs_s {
    vector_t one;
    vector_t thrsh;
    vector_t ratio;
    vector_t envA;
    vector_t envR;
    vector_t gainA;    
    vector_t gainR;
    vector_t gainM;  // Make Up Gain

    vector_t oenvA;
    vector_t oenvR;
    vector_t ogainA;
    vector_t ogainR;
    vector_t oratio;

    vector_t e;
    bool bypass[5];
} comp4_coeffs_t;

typedef struct comp4_states_s{
    vector_t gm;       // gain make-up
    vector_t gc;       // gain computer
    vector_t gs0;      // gain smoothing current
    vector_t gs1;      // gain smoothing previous
    vector_t aux;       
    vector_t env0;     // envelope current
    vector_t env1;     // envelope previous
} comp4_states_t;

/*******************************************************************************
 * Provides with the required data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   required data size for storing parameters
 * @param[out] coeffs_bytes   required data size for storing coefficients
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t comp4_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes);


/*******************************************************************************
 * Initializes params, coeffs and states to default values for the requested SR.
 * 
 * @param[in]  params       pointer to the pre-allocated params
 * @param[in]  coeffs       pointer to the pre-allocated coeffs
 * @param[in]  sample_rate  sampling rate
 * 
 * @return 0 if gain is initialized, non-zero error code otherwise
 ******************************************************************************/
int32_t comp4_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate);


/*******************************************************************************
 * Set single parameter 
 * 
 * @param[in] params    initialized params
 * @param[in] id        parameter ID
 * @param[in] value     parameter value
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t comp4_set_parameter(
    void*       params,
    int32_t     id,
    float       value);


/*******************************************************************************
 * Calculate coefficients, corresponding to the parameters.
 * 
 * @param[in] params    initialized params
 * @param[in] value     parameter value
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t comp4_update_coeffs(
    void const* params,
    void*       coeffs);



#endif
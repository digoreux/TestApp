#ifndef __COMP_FLT_CONTROL_H__
#define __COMP_FLT_CONTROL_H__

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "fractional.h"


typedef struct comp_stereo_s {
    flt left;
    flt right;
} comp_stereo_t;

typedef struct comp_params_s {
    flt threshold;
    flt ratio;

    flt tauAttack;      // gain
    flt tauRelease;

    flt makeUpGain;
    flt samplerate;

    flt tauEnvAtt;      //env
    flt tauEnvRel;
} comp_params_t;

typedef struct comp_coeffs_s {
    flt threshold;
    flt ratio;

    flt alphaAttack;    
    flt alphaRelease;

    flt attackEnv;
    flt releaseEnv;

    flt makeUpGain;
    flt samplerate;
} comp_coeffs_t;

typedef struct comp_states_s{

    comp_stereo_t x_sc;       // static characteristic

    comp_stereo_t g_c;

    comp_stereo_t g_s0;
    comp_stereo_t g_s1;

    comp_stereo_t g_m;      //make-up

    comp_stereo_t env0;
    comp_stereo_t env1;

} comp_states_t;

/*******************************************************************************
 * Provides with the required data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   required data size for storing parameters
 * @param[out] coeffs_bytes   required data size for storing coefficients
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t comp_effect_control_get_sizes(
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
int32_t comp_effect_control_initialize(
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
int32_t comp_effect_set_parameter(
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
int32_t comp_effect_update_coeffs(
    void const* params,
    void*       coeffs);



#endif
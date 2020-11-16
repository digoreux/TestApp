#ifndef __COMP4_FLT_CONTROL_H__
#define __COMP4_FLT_CONTROL_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "fractional.h"
#include "comp4_flt_control.h"
#include "comp_flt_control.h"

typedef struct comp4_stereo_s {
    flt left;
    flt right;
} comp4_stereo_t;

typedef struct comp4_bands_s {
    comp4_stereo_t * band1;
    comp4_stereo_t * band2;
    comp4_stereo_t * band3;
    comp4_stereo_t * band4;
} comp4_bands_t;

typedef struct comp4_params_s {
    comp_params_t comp1_p;
    comp_params_t comp2_p;
    comp_params_t comp3_p;
    comp_params_t comp4_p;
    uint32_t sample_rate;
    bool bypass;
} comp4_params_t;

typedef struct comp4_coeffs_s {
    comp_coeffs_t comp1_c;
    comp_coeffs_t comp2_c;
    comp_coeffs_t comp3_c;
    comp_coeffs_t comp4_c;
    bool bypass;
} comp4_coeffs_t;

typedef struct comp4_states_s{
    comp_states_t comp1_s;
    comp_states_t comp2_s;
    comp_states_t comp3_s;
    comp_states_t comp4_s;
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
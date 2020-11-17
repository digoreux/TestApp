#ifndef __CROSS_FXD_CONTROL_H__
#define __CROSS_FXD_CONTROL_H__

#define M_PI 3.14159265358979323846

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "fractional.h"

typedef struct cross_stereo_s {
    q31 left;
    q31 right;
} cross_stereo_t;

typedef struct cross_params_s {
    flt freq;
    uint32_t sample_rate;
    bool bypass;
} cross_params_t;

typedef struct cross_coeffs_s {
    q31 k0;     // 1st order
    q31 k1;     // 2nd order
    q31 k2;     // 2nd order
    bool bypass;
} cross_coeffs_t;

typedef struct cross_states_s {
    cross_stereo_t xn;
    cross_stereo_t y0[3];   // 1st order
    cross_stereo_t x0[3];   // 1st order
    cross_stereo_t x1[3];   // 1st order
    cross_stereo_t y1[2];   // 2nd order
    cross_stereo_t x2[2];   // 2nd order
    cross_stereo_t x3[2];   // 2nd order
    cross_stereo_t * band1;
    cross_stereo_t * band2;
} cross_states_t;

/*******************************************************************************
 * Provides with the required data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   required data size for storing parameters
 * @param[out] coeffs_bytes   required data size for storing coefficients
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t cross_control_get_sizes(
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
int32_t cross_control_initialize(
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
int32_t cross_set_parameter(
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
int32_t cross_update_coeffs(
    void const* params,
    void*       coeffs);



#endif
#ifndef __EQ_FXD_CONTROL_H__
#define __EQ_FXD_CONTROL_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "fractional.h"

#define M_PI  3.14159265358979323846

typedef enum {
	LP   = 0,
	HP   = 1,
	PEAK = 2,
	LSH  = 3,
	HSH	 = 4,
    OFF  = 5
} filter_types;


typedef struct eq_param_s {
    int32_t id;
    double  value;
} eq_param_t;

typedef struct eq_stereo_s {
    q31 left;
    q31 right;
} eq_stereo_t;

typedef struct eq_params_s {
    uint32_t sample_rate;
    eq_param_t  freq[10]; 
    eq_param_t  gain[10]; 
    eq_param_t     Q[10];   
    eq_param_t  type[10];
    bool bypass;
} eq_params_t;

typedef struct eq_coeffs_s {
    q31 b0[10];
    q31 b1[10];
    q31 b2[10];
    q31 a0[10];
    q31 a1[10];
    q31 a2[10];
    bool bypass;
} eq_coeffs_t;

typedef struct states_s {
   eq_stereo_t x0[10];
   eq_stereo_t x1[10];
   eq_stereo_t x2[10];
   eq_stereo_t y1[10];
   eq_stereo_t y2[10];
   eq_stereo_t error[10];
} eq_states_t;




/*******************************************************************************
 * Provides with the required data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   required data size for storing parameters
 * @param[out] coeffs_bytes   required data size for storing coefficients
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t eq_control_get_sizes(
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
int32_t eq_control_initialize(
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
int32_t eq_set_parameter(
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
int32_t eq_update_coeffs(
    void const* params,
    void*       coeffs);



#endif
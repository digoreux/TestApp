#ifndef __BIQUAD_FLT_CONTROL_H__
#define __BIQUAD_FLT_CONTROL_H__

#include "abstract_effect.h"

typedef flt stereo[2];

typedef struct coeffs_s {
    vector_t  b0;
    vector_t  b1;
    vector_t  b2;
    vector_t  a0;
    vector_t  a1;
    vector_t  a2;
    bool bypass;
} bq_coeffs_t;

typedef struct states_s {
   vector_t x0;
   vector_t x1;
   vector_t x2;
   vector_t y0;
} bq_states_t;

typedef struct bq_params_s {
    flt     Q;   
    flt  freq; 
    flt  gain; 
    flt  type;
    bool bypass;
    uint32_t sample_rate;
} bq_params_t;


/*******************************************************************************
 * Provides with the required data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   required data size for storing parameters
 * @param[out] coeffs_bytes   required data size for storing coefficients
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t bq_control_get_sizes(
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
int32_t bq_control_initialize(
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
int32_t bq_set_parameter(
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
int32_t bq_update_coeffs(
    void const* params,
    void*       coeffs);



#endif
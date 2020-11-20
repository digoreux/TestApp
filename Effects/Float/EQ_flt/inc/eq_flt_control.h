#ifndef __EQ_FLT_CONTROL_H__
#define __EQ_FLT_CONTROL_H__

#include "abstract_effect.h"
#include "biquad_flt_control.h"
#include "biquad_flt_process.h"

typedef flt stereo[2];

typedef struct eq_states_s {
    bq_states_t bq[10];
} eq_states_t;

typedef struct eq_params_s {
    bq_params_t bq[10];
    uint32_t sample_rate;
    bool bypass;
} eq_params_t;

typedef struct eq_coeffs_s {
    bq_coeffs_t bq[10];
    bool bypass;
} eq_coeffs_t;

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
#ifndef __CROSS4_FXD_CONTROL_H__
#define __CROSS4_FXD_CONTROL_H__

#define M_PI 3.14159265358979323846

#include "cross_fxd_control.h"

// typedef struct cross4_coeffs_s {
//     cross_coeffs_t cross1_c;
//     cross_coeffs_t cross2_c;
//     cross_coeffs_t cross3_c;
//     bool bypass;
// } cross4_coeffs_t;

// typedef struct cross4_params_s {
//     uint32_t sample_rate;
//     cross_params_t cross1_p;
//     cross_params_t cross2_p;
//     cross_params_t cross3_p;
//     bool bypass;
// } cross4_params_t;

// typedef struct bands_s {
//     cross_stereo_t * band1;
//     cross_stereo_t * band2;
//     cross_stereo_t * band3;
//     cross_stereo_t * band4;
// } bands_t;

// typedef struct cross4_states_s {
//     cross_states_t cross1_s;
//     cross_states_t cross2_s;
//     cross_states_t cross3_s;
//     bands_t bands;
//     cross_stereo_t xn;
//     cross_stereo_t y0[2];   // 1st order phase correction 
//     cross_stereo_t x0[2];   // 1st order phase correction
//     cross_stereo_t x1[2];   // 1st order phase correction
//     cross_stereo_t y1[2];   // 2nd order phase correction
//     cross_stereo_t x2[2];   // 2nd order phase correction
//     cross_stereo_t x3[2];   // 2nd order phase correction
// } cross4_states_t;



/*******************************************************************************
 * Provides with the required data sizes for parameters and coefficients.
 *   It is caller responsibility to allocate enough memory (bytes) for them.
 * 
 * @param[out] params_bytes   required data size for storing parameters
 * @param[out] coeffs_bytes   required data size for storing coefficients
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t cross4_control_get_sizes(
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
int32_t cross4_control_initialize(
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
int32_t cross4_set_parameter(
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
int32_t cross4_update_coeffs(
    void const* params,
    void*       coeffs);



#endif
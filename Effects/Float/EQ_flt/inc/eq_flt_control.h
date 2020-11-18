#ifndef __EQ_FLT_CONTROL_H__
#define __EQ_FLT_CONTROL_H__

#include "abstract_effect.h"

typedef enum {
	LP   = 0,
	HP   = 1,
	PEAK = 2,
	LSH  = 3,
	HSH	 = 4,
    OFF  = 5
} filter_types;

typedef flt stereo[2];

typedef struct param_s {
    int32_t id;
    double  value;   
} param_t;

typedef struct eq_states_s {
    xmm   ms[4][10];
    stereo s[4][10];    //x0,x1,x2,y0
} eq_states_t;

typedef struct eq_params_s {
    uint32_t sample_rate;
    param_t  freq[10]; 
    param_t  gain[10]; 
    param_t     Q[10];   
    param_t  type[10];
    bool bypass;
} eq_params_t;

typedef struct eq_coeffs_s {
    flt  k[6][10]; //a0,a1,a2,b0,b1,b2
    xmm mk[6][10];
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
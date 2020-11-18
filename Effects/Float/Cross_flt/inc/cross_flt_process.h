#ifndef __CROSS_FLT_PROCESS_H__
#define __CROSS_FLT_PROCESS_H__

#include "cross_flt_control.h"

/*******************************************************************************
 * Provides with the required data sizes for states. It is caller responsibility
 *   to allocate enough memory (bytes) for them.
 * 
 * @param[out] states_bytes required data size for storing states
 * 
 * @return 0 if success, non-zero error code otherwise
 ******************************************************************************/
int32_t cross_process_get_sizes(
    size_t*     states_bytes);
 

/*******************************************************************************
 * Reset internal states. Configuration remains the same.
 * 
 * @param[in] coeffs        initialized coeffs
 * @param[in] states        initialized states
 * 
 * @return 0 on success, otherwise fail
 ******************************************************************************/
int32_t cross_reset(
    void const* coeffs,
    void*       states);


/*******************************************************************************
 * Process all available data in the input audio buffer (in-place processing).
 *   Samples are presented in the interleaved manner [L, R, L, R, ...].
 * 
 * @param[in] coeffs        initialized coeffs
 * @param[in] states        initialized states
 * @param[in,out] audio     array of input samples in the interleaved manner
 * @param[in] samples_count count of multichannel samples (L, R pairs) in audio
 * 
 * @return 0 on success, otherwise fail
 ******************************************************************************/
int32_t cross_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    stereo_t* band1,
    stereo_t* band2,
    size_t samples_count);


#endif

#ifndef __BIQUAD_FXD_STRUCTURE_H__
#define __BIQUAD_FXD_STRUCTURE_H__

#include "abstract_effect.h"

typedef struct bq_coeffs_s {
    q31  b0;
    q31  b1;
    q31  b2;
    q31  a0;
    q31  a1;
    q31  a2;
    bool bypass;
} bq_coeffs_t;

typedef struct bq_states_s {
   stereo_t x0;
   stereo_t x1;
   stereo_t x2;
   stereo_t y0;
   stereo_t y1;
   stereo_t y2;
   stereo_t error;
} bq_states_t;

typedef struct bq_params_s {
    flt     Q;   
    flt  freq; 
    flt  gain; 
    flt  type;
    bool bypass;
    uint32_t sample_rate;
} bq_params_t;

#endif

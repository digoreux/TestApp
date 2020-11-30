#ifndef __BIQUAD_FLT_STRUCTURE_H__
#define __BIQUAD_FLT_STRUCTURE_H__

#include "abstract_effect.h"

typedef struct coeffs_s {
    vector_t  b0;
    vector_t  b1;
    vector_t  b2;
    vector_t  a0;
    vector_t  a1;
    vector_t  a2;

    flt  sb0;
    flt  sb1;
    flt  sb2;
    flt  sa0;
    flt  sa1;
    flt  sa2;
    bool bypass;
} bq_coeffs_t;

typedef struct states_s {
   vector_t x0;
   vector_t x1;
   vector_t x2;
   vector_t y0;

   stereo_t sx0;
   stereo_t sx1;
   stereo_t sx2;
   stereo_t sy0;
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

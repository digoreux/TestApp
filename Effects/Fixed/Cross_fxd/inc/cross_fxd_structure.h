#ifndef __CROSS_FXD_STRUCTURE_H__
#define __CROSS_FXD_STRUCTURE_H__

#include "abstract_effect.h"


typedef struct cross_params_s {
    flt freq;
    uint32_t sample_rate;
    bool bypass;
} cross_params_t;

typedef struct cross_coeffs_s {
    q31 k0;     
    q31 k1;     
    q31 k2;     
    bool bypass;
} cross_coeffs_t;

typedef struct cross_states_s {
    stereo_t xn;
    stereo_t y0[2];   
    stereo_t x0[2];   
    stereo_t x1[2];   

    stereo_t * band1;
    stereo_t * band2;
} cross_states_t;


typedef struct cross4_coeffs_s {
    cross_coeffs_t cross[3];
    vector_t k0;
    vector_t k1;
    vector_t k2;
    bool bypass;
} cross4_coeffs_t;

typedef struct cross4_params_s {
    uint32_t sample_rate;
    cross_params_t cross[3];
    bool bypass;
} cross4_params_t;

typedef struct cross4_states_s {
    cross_states_t cross[3];
    vector_t xn[3];   // 2nd order phase correction 
    vector_t y0[3];   // 2nd order phase correction 
    vector_t x0[3];   // 2nd order phase correction
    vector_t x1[3];   // 2nd order phase correction

    stereo_t b1[32];
    stereo_t b2[32];
    stereo_t b3[32];
    stereo_t b4[32];
} cross4_states_t;

#endif

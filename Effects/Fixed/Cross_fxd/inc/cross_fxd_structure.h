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
    stereo_t x0[2];   
    stereo_t y0[2];
    stereo_t x1[2];   
    stereo_t y1[2];   
} cross_states_t;

typedef struct cross4_coeffs_s {
    cross_coeffs_t cross[3];
    q31 k0[2];
    q31 k1[2];
    q31 k2[2];
    bool bypass;
} cross4_coeffs_t;

typedef struct cross4_params_s {
    cross_params_t cross[3];
    uint32_t sample_rate;
    bool bypass;
} cross4_params_t;

typedef struct cross4_states_s {
    cross_states_t cross[3];
    stereo_t x0[2];   // 2nd order phase correction
    stereo_t y0[2];   // 2nd order phase correction 
    stereo_t x1[2];   // 2nd order phase correction
    stereo_t y1[2];   // 2nd order phase correction 

    stereo_t b1[FRAME_COUNT];
    stereo_t b2[FRAME_COUNT];
    stereo_t b3[FRAME_COUNT];
    stereo_t b4[FRAME_COUNT];
} cross4_states_t;

#endif

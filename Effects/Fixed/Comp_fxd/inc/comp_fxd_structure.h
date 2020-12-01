#ifndef __COMP_FXD_STRUCTURE_H__
#define __COMP_FXD_STRUCTURE_H__

#include "abstract_effect.h"

typedef struct comp_params_s {
    flt thrsh;
    flt ratio;
    flt tAttack;      // gain
    flt tRelease;
    flt tEnvAttack;      //env
    flt tEnvRelease;
    flt makeUpGain;
    uint32_t sample_rate;
    bool downward;
    bool bypass;

} comp_params_t;

typedef struct comp_coeffs_s {
    q31 thrsh;
    q31 ratio;
    q31 envA;
    q31 envR;
    q31 gainA;    
    q31 gainR;
    q31 gainM;  // Make Up Gain
    q31 sample_rate;
    bool bypass;
} comp_coeffs_t;

typedef struct comp_states_s{
    q31 gm;      // gain make-up
    q31 gc;      // gain computer
    q31 gs0;     // gain smoothing current
    q31 gs1;     // gain smoothing previous
    q31 env0;     // envelope current
    q31 env1;     // envelope previous
} comp_states_t;

#endif
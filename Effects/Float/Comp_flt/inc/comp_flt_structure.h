#ifndef __COMP_FLT_STRUCTURE_H__
#define __COMP_FLT_STRUCTURE_H__

#include "abstract_effect.h"

typedef struct comp_params_s {
    flt thrsh;
    flt ratio;
    flt tAttack;         // gain
    flt tRelease;
    flt tEnvAttack;      //env
    flt tEnvRelease;
    flt makeUpGain;
    uint32_t sample_rate;
    bool downward;
    bool bypass;
} comp_params_t;

typedef struct comp_coeffs_s {
    flt thrsh;
    flt ratio;
    flt envA;
    flt envR;
    flt gainA;    
    flt gainR;
    flt gainM;  // Make Up Gain
    bool bypass;
} comp_coeffs_t;

typedef struct comp_states_s{
    flt gm;       // gain make-up
    flt gc;       // gain computer
    flt gs0;      // gain smoothing current
    flt gs1;      // gain smoothing previous
    flt env0;     // envelope current
    flt env1;     // envelope previous
} comp_states_t;

#endif
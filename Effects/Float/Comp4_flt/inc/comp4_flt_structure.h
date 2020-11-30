#ifndef __COMP4_FLT_STRUCTURE_H__
#define __COMP4_FLT_STRUCTURE_H__

#include "abstract_effect.h"

typedef struct comp4_params_s {
    flt thrsh[4];
    flt ratio[4];
    flt tAttack[4];         // gain
    flt tRelease[4];
    flt tEnvAttack[4];      //env
    flt tEnvRelease[4];
    flt makeUpGain[4];
    uint32_t sample_rate;
    bool downward[4];
    bool bypass[5];
} comp4_params_t;

typedef struct comp4_coeffs_s {
    vector_t one;
    vector_t thrsh;
    vector_t ratio;
    vector_t envA;
    vector_t envR;
    vector_t gainA;    
    vector_t gainR;
    vector_t gainM;  // Make Up Gain

    vector_t oenvA;
    vector_t oenvR;
    vector_t ogainA;
    vector_t ogainR;
    vector_t oratio;

    vector_t e;
    bool bypass[5];
} comp4_coeffs_t;

typedef struct comp4_states_s{
    vector_t gm;       // gain make-up
    vector_t gc;       // gain computer
    vector_t gs0;      // gain smoothing current
    vector_t gs1;      // gain smoothing previous
    vector_t aux;       
    vector_t env0;     // envelope current
    vector_t env1;     // envelope previous
} comp4_states_t;

#endif

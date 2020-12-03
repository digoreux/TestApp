#ifndef __COMP4_FXD_STRUCTURE_H__
#define __COMP4_FXD_STRUCTURE_H__

#include "abstract_effect.h"
#include "comp_fxd_structure.h"
#include "cross_fxd_structure.h"


typedef struct comp4_params_s {
    comp_params_t comp[4];
    uint32_t sample_rate;
    bool bypass;
} comp4_params_t;

typedef struct comp4_coeffs_s {
    comp_coeffs_t comp[4];
    bool bypass;
} comp4_coeffs_t;

typedef struct comp4_states_s{
    comp_states_t comp[4];
} comp4_states_t;

#endif
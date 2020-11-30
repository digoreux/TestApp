#ifndef __COMP4S_FLT_STRUCTURE_H__
#define __COMP4S_FLT_STRUCTURE_H__

#include "abstract_effect.h"
#include "comp_flt_structure.h"

typedef struct comp4s_params_s {
    comp_params_t comp1_p;
    comp_params_t comp2_p;
    comp_params_t comp3_p;
    comp_params_t comp4_p;
    uint32_t sample_rate;
    bool bypass;
} comp4s_params_t;

typedef struct comp4s_coeffs_s {
    comp_coeffs_t comp1_c;
    comp_coeffs_t comp2_c;
    comp_coeffs_t comp3_c;
    comp_coeffs_t comp4_c;
    bool bypass;
} comp4s_coeffs_t;

typedef struct comp4s_states_s{
    comp_states_t comp1_s;
    comp_states_t comp2_s;
    comp_states_t comp3_s;
    comp_states_t comp4_s;
} comp4s_states_t;

#endif

#ifndef __EFFECT_STRUCTURE_H__
#define __EFFECT_STRUCTURE_H__

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "abstract_effect.h"
#include "eq_flt_control.h"
#include "comp_flt_control.h"
#include "comp4_flt_control.h"
#include "comp4s_flt_control.h"
#include "cross4_flt_control.h"
#include "biquad_flt_control.h"
#include "expand_flt_control.h"

#include "eq_flt_process.h"
#include "comp_flt_process.h"
#include "comp4_flt_process.h"
#include "comp4s_flt_process.h"
#include "cross4_flt_process.h"
#include "expand_flt_process.h"
#include "biquad_flt_process.h"

typedef struct params_t {
    bq_params_t bq;
    eq_params_t eq;
    comp_params_t comp;
    cross_params_t cross;
    comp4_params_t comp4;
    comp4s_params_t comp4s;
    cross4_params_t cross4;
    expand_params_t expand;
} params_t;

typedef struct coeffs_t {
    bq_coeffs_t bq;
    eq_coeffs_t eq;
    comp_coeffs_t comp;
    cross_coeffs_t cross;
    comp4_coeffs_t comp4;
    comp4s_coeffs_t comp4s;
    cross4_coeffs_t cross4;
    expand_coeffs_t expand;
} coeffs_t;

typedef struct states_t {
    bq_states_t bq;
    eq_states_t eq;
    comp_states_t comp;
    cross_states_t cross;
    comp4_states_t comp4;
    comp4s_states_t comp4s;
    cross4_states_t cross4;
    expand_states_t expand;
} states_t;

#endif

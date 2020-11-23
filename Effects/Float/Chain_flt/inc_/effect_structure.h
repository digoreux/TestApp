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
#include "cross4_flt_control.h"
#include "biquad_flt_control.h"
#include "expand_flt_control.h"

#include "eq_flt_process.h"
#include "comp_flt_process.h"
#include "comp4_flt_process.h"
#include "cross4_flt_process.h"
#include "expand_flt_process.h"
#include "biquad_flt_process.h"

typedef struct params_t {
    bq_params_t bq_p;
    eq_params_t eq_p;
    comp_params_t comp_p;
    cross_params_t cross_p;
    comp4_params_t comp4_p;
    cross4_params_t cross4_p;
    expand_params_t expand_p;
} params_t;

typedef struct coeffs_t {
    bq_coeffs_t bq_c;
    eq_coeffs_t eq_c;
    comp_coeffs_t comp_c;
    cross_coeffs_t cross_c;
    comp4_coeffs_t comp4_c;
    cross4_coeffs_t cross4_c;
    expand_coeffs_t expand_c;
} coeffs_t;

typedef struct states_t {
    bq_states_t bq_s;
    eq_states_t eq_s;
    comp_states_t comp_s;
    cross_states_t cross_s;
    comp4_states_t comp4_s;
    cross4_states_t cross4_s;
    expand_states_t expand_s;
} states_t;

#endif

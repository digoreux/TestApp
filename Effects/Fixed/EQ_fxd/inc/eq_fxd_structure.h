#ifndef __EQ_FXD_STRUCTURE_H__
#define __EQ_FXD_STRUCTURE_H__

#include "abstract_effect.h"
#include "biquad_fxd_structure.h"

typedef struct eq_params_s {
    uint32_t sample_rate;
    bq_params_t bq[10];
    bool bypass;
} eq_params_t;

typedef struct eq_coeffs_s {
    bq_coeffs_t bq[10];
    bool bypass;
} eq_coeffs_t;

typedef struct states_s {
    bq_states_t bq[10];
} eq_states_t;


#endif

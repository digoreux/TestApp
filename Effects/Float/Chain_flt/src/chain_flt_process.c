#include "chain_flt_process.h"
#include "comp_flt_process.h"
#include "eq_flt_process.h"

typedef struct stereo_s {
    flt left;
    flt right;
} stereo_t;

typedef struct params_t {
    eq_params_t   eq_p;
    comp_params_t comp_p;
} params_t;

typedef struct coeffs_t {
    eq_coeffs_t   eq_c;
    comp_coeffs_t comp_c;
} coeffs_t;

typedef struct states_t {
    eq_states_t   eq_s;
    comp_states_t comp_s;
} states_t;

int32_t effect_process_get_sizes(
    size_t*     states_bytes)
{   
    *states_bytes = sizeof(states_t);
    return 0;
}

int32_t effect_reset(
    void const* coeffs,
    void*       states)
{   
    coeffs_t *c = (coeffs_t*)coeffs;
    states_t *s = (states_t*)states;

    eq_effect_reset(&c->eq_c, &s->eq_s);
    comp_effect_reset(&c->comp_c, &s->comp_s);

    return 0;
}

int32_t effect_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{   
    coeffs_t *c = (coeffs_t*)coeffs;
    states_t *s = (states_t*)states;
    stereo_t *a = (stereo_t*)audio;

    comp_effect_process(&c->comp_c, &s->comp_s, audio, samples_count);
    eq_effect_process(&c->eq_c, &s->eq_s, audio, samples_count);

    return 0;
}
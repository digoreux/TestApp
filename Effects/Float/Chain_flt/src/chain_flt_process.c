#include "chain_flt_process.h"
#include "cross_flt_process.h"
#include "comp_flt_process.h"
#include "eq_flt_process.h"

typedef struct stereo_s {
    flt left;
    flt right;
} stereo_t;

typedef struct params_t {
    eq_params_t   eq_p;
    comp_params_t comp_p;
    cross_params_t cross_p;
} params_t;

typedef struct coeffs_t {
    eq_coeffs_t   eq_c;
    comp_coeffs_t comp_c;
    cross_coeffs_t cross_c;
} coeffs_t;

typedef struct states_t {
    eq_states_t   eq_s;
    comp_states_t comp_s;
    cross_states_t cross_s;
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

    eq_reset(&c->eq_c, &s->eq_s);
    comp_reset(&c->comp_c, &s->comp_s);
    cross_reset(&c->cross_c, &s->cross_s);

    s->cross_s.band1 = malloc(sizeof(stereo_t) * 512);
    s->cross_s.band2 = malloc(sizeof(stereo_t) * 512);
    s->cross_s.band3 = malloc(sizeof(stereo_t) * 512);
    s->cross_s.band4 = malloc(sizeof(stereo_t) * 512);

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

    // comp_process(&c->comp_c, &s->comp_s, audio, samples_count);
    // eq_process(&c->eq_c, &s->eq_s, audio, samples_count);
    cross_process(&c->cross_c, &s->cross_s, audio, samples_count);
    

    return 0;
}
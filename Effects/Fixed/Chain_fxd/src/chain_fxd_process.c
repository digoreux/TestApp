#include "chain_fxd_process.h"
#include "cross_fxd_process.h"
#include "comp_fxd_process.h"
#include "eq_fxd_process.h"
// #include "cross4_fxd_process.h"
// #include "cross_fxd_process.h"
// #include "comp4_fxd_process.h"

typedef struct stereo_s {
    q31 left;
    q31 right;
} stereo_t;

typedef struct params_t {
    eq_params_t   eq_p;
    comp_params_t comp_p;
    // comp4_params_t comp4_p;
    // cross4_params_t cross4_p;
} params_t;

typedef struct coeffs_t {
    eq_coeffs_t   eq_c;
    comp_coeffs_t comp_c;
    // comp4_coeffs_t comp4_c;
    // cross4_coeffs_t cross4_c;
} coeffs_t;

typedef struct states_t {
    eq_states_t   eq_s;
    comp_states_t comp_s;
    // comp4_states_t comp4_s;
    // cross4_states_t cross4_s;
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
 
    // eq_reset(&c->eq_c, &s->eq_s);
    // comp_reset(&c->comp_c, &s->comp_s);
    // cross4_reset(&c->cross4_c, &s->cross4_s);
    // comp4_reset(&c->comp4_c, &s->cross4_s);
    // s->cross4_s.bands.band1 = malloc(sizeof(stereo_t) * 512);
    // s->cross4_s.bands.band2 = malloc(sizeof(stereo_t) * 512);
    // s->cross4_s.bands.band3 = malloc(sizeof(stereo_t) * 512);
    // s->cross4_s.bands.band4 = malloc(sizeof(stereo_t) * 512);

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

    eq_process(&c->eq_c, &s->eq_s, audio, samples_count);
    comp_process(&c->comp_c, &s->comp_s, audio, samples_count);
    // cross4_process(&c->cross4_c, &s->cross4_s, audio, samples_count);
    // comp4_process(&c->comp4_c, &s->comp4_s, &s->cross4_s.bands, samples_count);    
    // mix(audio, &s->cross4_s.bands, samples_count);

    return 0;
}
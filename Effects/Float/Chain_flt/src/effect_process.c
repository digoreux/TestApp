#include "effect_process.h"
#include "effect_structure.h"

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
    bq_reset(&c->bq_c, &s->bq_s);
    comp_reset(&c->comp_c, &s->comp_s);
    expand_reset(&c->expand_c, &s->expand_s);
    cross4_reset(&c->cross4_c, &s->cross4_s);
    comp4_reset(&c->comp4_c, &s->cross4_s);
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

    // bq_process(&c->bq_c, &s->bq_s, audio, samples_count);
    eq_process(&c->eq_c, &s->eq_s, audio, samples_count);
    //  comp_process(&c->comp_c, &s->comp_s, audio, samples_count);
    //  cross4_process(&c->cross4_c, &s->cross4_s, audio, samples_count);
     //comp4_process(&c->comp4_c, &s->comp4_s, &s->cross4_s.bands, samples_count);    
    //  mix(audio, &s->cross4_s.bands, samples_count);
    // expand_process(&c->expand_c, &s->expand_s, audio, samples_count);

    return 0;
}
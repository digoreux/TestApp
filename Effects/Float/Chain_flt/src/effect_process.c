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
 
    eq_reset(&c->eq, &s->eq);
    bq_reset(&c->bq, &s->bq);
    cross_reset(&c->cross, &s->cross);
    cross4_reset(&c->cross4, &s->cross4); 
    // comp_reset(&c->comp, &s->comp);
    // expand_reset(&c->expand, &s->expand);
    // comp4_reset(&c->comp4, &s->cross4);
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

    for(uint32_t i = 0, n = 0; i < samples_count; i += FRAME_COUNT, n++)
    {
        // bq_process(&c->bq, &s->bq, audio, FRAME_COUNT, n);
        // eq_process(&c->eq, &s->eq, audio, FRAME_COUNT, n);
        // cross_process(&c->cross, &s->cross, audio, s->cross4.b1, s->cross4.b2, FRAME_COUNT, n);
        cross4_process(&c->cross4, &s->cross4, audio, FRAME_COUNT, n);
        // mix2(audio, &s->cross4.b1, &s->cross4.b3, FRAME_COUNT, n);
        mix(audio, &s->cross4, FRAME_COUNT, n);
    }

    //  comp4_process(&c->comp4, &s->comp4, &s->cross4.bands, samples_count);    
    //  expand_process(&c->expand, &s->expand, audio, samples_count);
    //  comp_process(&c->comp, &s->comp, audio, samples_count);

    return 0;
}
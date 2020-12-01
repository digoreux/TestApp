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
    comp_reset(&c->comp, &s->comp);
    comp4_reset(&c->comp4, &s->comp4);
    cross4_reset(&c->cross4, &s->cross4); 
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

    for(uint32_t i = 0; i < samples_count; i += FRAME_COUNT)
    {
        eq_process(&c->eq, &s->eq, &a[i], FRAME_COUNT);
        comp_process(&c->comp, &s->comp, &a[i], FRAME_COUNT);
        cross4_process(&c->cross4, &s->cross4, &a[i], FRAME_COUNT);
        comp4_process(&c->comp4, &s->comp4, &s->cross4, FRAME_COUNT);    
        mix(&a[i], &s->cross4, FRAME_COUNT);
        
        // bq_process(&c->bq, &s->bq, &a[i], FRAME_COUNT);
    }


    return 0;
}
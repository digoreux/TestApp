#include "comp4_flt_process.h"
#include "comp_flt_process.h"

int32_t comp4_process_get_sizes(
    size_t*     states_bytes)
{
    *states_bytes = sizeof(comp4_states_t);

    return 0;
}

int32_t comp4_reset(
    void const* coeffs,
    void*       states)
{
    comp4_states_t* s = (comp4_states_t*)states;
    comp4_states_t* c = (comp4_states_t*)coeffs;

    comp_reset(&c->comp1_s, &s->comp1_s);
    comp_reset(&c->comp2_s, &s->comp2_s);
    comp_reset(&c->comp3_s, &s->comp3_s);
    comp_reset(&c->comp4_s, &s->comp4_s);

    return 0;
}


int32_t comp4_process(
    void const* coeffs,
    void*       states,
    void*       bands,
    size_t      samples_count)
{
    comp4_coeffs_t* c = (comp4_coeffs_t*)coeffs;
    comp4_states_t* s = (comp4_states_t*)states;
    comp4_bands_t * b = (comp4_bands_t*)bands;

    if(!c->bypass)
    {
        if(!c->comp1_c.bypass) comp_process(&c->comp1_c, &s->comp1_s, b->band1, samples_count);
        if(!c->comp2_c.bypass) comp_process(&c->comp2_c, &s->comp2_s, b->band2, samples_count);
        if(!c->comp3_c.bypass) comp_process(&c->comp3_c, &s->comp3_s, b->band3, samples_count);
        if(!c->comp4_c.bypass) comp_process(&c->comp4_c, &s->comp4_s, b->band4, samples_count);
    }
   
    return 0;
}


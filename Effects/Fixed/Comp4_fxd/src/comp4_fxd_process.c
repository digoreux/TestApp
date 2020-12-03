#include "comp4_fxd_process.h"

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
    comp4_coeffs_t* c = (comp4_coeffs_t*)coeffs;

    for(uint32_t i = 0; i < 4; i++)
        comp_reset(&c->comp[i], &s->comp[i]);

    return 0;
}


int32_t comp4_process(
    void const* coeffs,
    void*       states,
    void*       bands,
    size_t      samples_count)
{
    comp4_coeffs_t  * c = (comp4_coeffs_t*)coeffs;
    comp4_states_t  * s = (comp4_states_t*)states;
    cross4_states_t * b = (cross4_states_t*)bands;
    if(!c->bypass)
    {   
        if(!c->comp[0].bypass) comp_process(&c->comp[0], &s->comp[0], b->b1, samples_count);
        if(!c->comp[1].bypass) comp_process(&c->comp[1], &s->comp[1], b->b2, samples_count);
        if(!c->comp[2].bypass) comp_process(&c->comp[2], &s->comp[2], b->b3, samples_count);
        if(!c->comp[3].bypass) comp_process(&c->comp[3], &s->comp[3], b->b4, samples_count);
            
    }
   
    return 0;
}


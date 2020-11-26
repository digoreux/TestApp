#include "comp4s_flt_process.h"
#include "comp_flt_process.h"
#include "cross4_flt_control.h"

int32_t comp4s_process_get_sizes(
    size_t*     states_bytes)
{
    *states_bytes = sizeof(comp4s_states_t);

    return 0;
}

int32_t comp4s_reset(
    void const* coeffs,
    void*       states)
{
    comp4s_states_t* s = (comp4s_states_t*)states;
    comp4s_coeffs_t* c = (comp4s_coeffs_t*)coeffs;

    comp_reset(&c->comp1_c, &s->comp1_s);
    comp_reset(&c->comp2_c, &s->comp2_s);
    comp_reset(&c->comp3_c, &s->comp3_s);
    comp_reset(&c->comp4_c, &s->comp4_s);

    return 0;
}


int32_t comp4s_process(
    void const* coeffs,
    void*       states,
    void*       bands,
    size_t      samples_count,
    size_t      frames_count)
{
    comp4s_coeffs_t  * c = (comp4s_coeffs_t*)coeffs;
    comp4s_states_t  * s = (comp4s_states_t*)states;
    cross4_states_t * b = (cross4_states_t *)bands;
    if(!c->bypass)
    {
        if(!c->comp1_c.bypass) comp_process_frame(&c->comp1_c, &s->comp1_s, b->b1, samples_count, frames_count);
        if(!c->comp2_c.bypass) comp_process_frame(&c->comp2_c, &s->comp2_s, b->b2, samples_count, frames_count);
        if(!c->comp3_c.bypass) comp_process_frame(&c->comp3_c, &s->comp3_s, b->b3, samples_count, frames_count);
        if(!c->comp4_c.bypass) comp_process_frame(&c->comp4_c, &s->comp4_s, b->b4, samples_count, frames_count);
    }
   
    return 0;
}


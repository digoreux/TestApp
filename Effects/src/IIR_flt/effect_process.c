#include "effect_process.h"
#include "effect_control.h"


typedef struct stereo {
    float left;
    float right;
} channel;



int32_t effect_process_get_sizes(
    size_t*     states_bytes)
{
    return 0;
}

int32_t effect_reset(
    void const* coeffs,
    void*       states)
{   

    states_t * s = (states_t*)states;
    s->x0 = 0;
    s->x1 = 0;
    s->x2 = 0;
    s->y1 = 0;
    s->y2 = 0;

    return 0;
}

int32_t effect_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{   
    double acc = 0;
    coeffs_t *c = (coeffs_t*)coeffs;
    states_t *s = (states_t*)states;

    for (int i = 0; i < samples_count; i++)
    {   
        s->x0 = ((float*)audio)[i];
        acc = c->b0 * s->x0 + c->b1 * s->x1 + c->b2 * s->x2 -
            c->a1 * s->y1 - c->a2 * s->y2;

        s->x2 = s->x1;
        s->x1 = s->x0;

        s->y2 = s->y1;
        s->y1 = acc;

        ((float*)audio)[i] = acc;
    }
   
   
    return 0;
}


#include "effect_process.h"
#include "effect_control.h"


typedef struct stereo {
    float left;
    float right;
} stereo_t;

typedef struct states_s {
   stereo_t x0;
   stereo_t x1;
   stereo_t x2;
   stereo_t y1;
   stereo_t y2;
} states_t;

typedef struct coeffs_s {
    double  b0;
    double  b1;
    double  b2;
    double  a0;
    double  a1;
    double  a2;
} coeffs_t;

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

    states_t * s = (states_t*)states;
    s->x0.left = 0;
    s->x1.left = 0;
    s->x2.left = 0;
    s->y1.left = 0;
    s->y2.left = 0;

    s->x0.right = 0;
    s->x1.right = 0;
    s->x2.right = 0;
    s->y1.right = 0;
    s->y2.right = 0;

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
    stereo_t *a = (stereo_t*)audio;

    for (int i = 0; i < samples_count; i++)
    {   
        s->x1 = 
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


#include "effect_process.h"
#include "effect_control.h"
#include "fractional.h"
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
    double lacc;
    double racc;
    coeffs_t *c = (coeffs_t*)coeffs;
    states_t *s = (states_t*)states;
    stereo_t *a = (stereo_t*)audio;

    for (int i = 0; i < samples_count; i++)
    {   
        lacc = 0;
        s->x0.left = a[i].left;

        lacc = macf(c->b0, s->x0.left, lacc);
        lacc = macf(c->b1, s->x1.left, lacc);
        lacc = macf(c->b2, s->x2.left, lacc);
        lacc = msubf(c->a1, s->y1.left, lacc); 
        lacc = msubf(c->a2, s->y2.left, lacc);

        s->x2.left = s->x1.left;
        s->x1.left = s->x0.left;

        s->y2.left = s->y1.left;
        s->y1.left = lacc;
         
        a[i].left = lacc;  

        racc = 0;
        s->x0.right = a[i].right;

        racc = macf(c->b0, s->x0.right, racc);
        racc = macf(c->b1, s->x1.right, racc);
        racc = macf(c->b2, s->x2.right, racc);
        racc = msubf(c->a1, s->y1.right, racc);
        racc = msubf(c->a2, s->y2.right, racc);

        s->x2.right = s->x1.right;
        s->x1.right = s->x0.right;

        s->y2.right = s->y1.right;
        s->y1.right = racc;
         
        a[i].right = racc;
    }
   
   
    return 0;
}


#include "effect_process.h"
#include "effect_control.h"
#include "fractional.h"


typedef struct stereo_s {
    q31 left;
    q31 right;
} stereo_t;

typedef struct states_s {
   stereo_t x0;
   stereo_t x1;
   stereo_t x2;
   stereo_t y1;
   stereo_t y2;
} states_t;

typedef struct coeffs_s {
    q31  b0;
    q31  b1;
    q31  b2;
    q31  a0;
    q31  a1;
    q31  a2;
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
    q63 acc = 0;
    coeffs_t *c = (coeffs_t*)coeffs;
    states_t *s = (states_t*)states;
    stereo_t *a = (stereo_t*)audio;

    for (int i = 0; i < samples_count; i++)
    {   
        s->x0.left  = a[i].left;
        s->x0.right = a[i].right;

        acc = mac_q31(c->b0, s->x0.left, acc);
        acc = mac_q31(c->b1, s->x1.left, acc);
        acc = mac_q31(c->b2, s->x2.left, acc);
        acc = msub_q31(c->a1, s->y1.left, acc); 
        acc = msub_q31(c->a2, s->y2.left, acc);

        acc >>= 30;
        a[i].left = (q31)acc;  

        s->x2.left = s->x1.left;
        s->x1.left = s->x0.left;
        s->y2.left = s->y1.left;
        s->y1.left = acc;

        acc = mac_q31(c->b0, s->x0.right, acc);
        acc = mac_q31(c->b1, s->x1.right, acc);
        acc = mac_q31(c->b2, s->x2.right, acc);
        acc = msub_q31(c->a1, s->y1.right, acc);
        acc = msub_q31(c->a2, s->y2.right, acc);

        acc >>= 30;
        a[i].right = (q31)acc;  
        
        s->x2.right = s->x1.right;
        s->x1.right = s->x0.right;
        s->y2.right = s->y1.right;
        s->y1.right = acc;

    }
    return 0;
}


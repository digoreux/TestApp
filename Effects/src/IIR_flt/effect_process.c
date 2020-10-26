#include "effect_process.h"
#include "effect_control.h"
#include "fractional.h"
typedef struct stereo {
    flt left;
    flt right;
} stereo_t;

typedef struct states_s {
   stereo_t x0;
   stereo_t x1;
   stereo_t x2;
   stereo_t y1;
   stereo_t y2;
   stereo_t s11;
   stereo_t s12;
   stereo_t s21;
   stereo_t s22;
} states_t;

typedef struct coeffs_s {
    flt  b0;
    flt  b1;
    flt  b2;
    flt  a0;
    flt  a1;
    flt  a2;
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

    s->s11.left = 0;
    s->s12.left = 0;
    s->s21.left = 0;
    s->s22.left = 0;
    s->s11.right = 0;
    s->s12.right = 0;
    s->s21.right = 0;
    s->s22.right = 0;
    return 0;
}

int32_t effect_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{   
    flt acc = 0;
    coeffs_t *c = (coeffs_t*)coeffs;
    states_t *s = (states_t*)states;
    stereo_t *a = (stereo_t*)audio;

    for (size_t i = 0; i < samples_count; i++)
    {   
        /*  
            DIRECT FORM I
        */
       
        // s->x0.left  = a[i].left;
        // s->x0.right = a[i].right;

        // acc = 0;
        // acc = macf(c->b0, s->x0.left, acc);
        // acc = macf(c->b1, s->x1.left, acc);
        // acc = macf(c->b2, s->x2.left, acc);
        // acc = msubf(c->a1, s->y1.left, acc);
        // acc = msubf(c->a2, s->y2.left, acc);

        // a[i].left = acc;

        // s->x2.left = s->x1.left;
        // s->x1.left = s->x0.left;
        // s->y2.left = s->y1.left;
        // s->y1.left = acc;

        // acc = 0;
        // acc = macf(c->b0, s->x0.right, acc);
        // acc = macf(c->b1, s->x1.right, acc);
        // acc = macf(c->b2, s->x2.right, acc);
        // acc = msubf(c->a1, s->y1.right, acc);
        // acc = msubf(c->a2, s->y2.right, acc);

        // s->x2.right = s->x1.right;
        // s->x1.right = s->x0.right;
        // s->y2.right = s->y1.right;
        // s->y1.right = acc;

        // a[i].right = acc;

        /*  
            DIRECT FORM TRANSPOSED II
        */

        acc = 0;
        acc  =  macf(c->b0, s->x0.left, s->s12.left);

        s->s11.left =  macf(c->b1, s->x0.left, s->s22.left);
        s->s11.left = msubf(c->a1, acc, s->s11.left);

        s->s21.left =  macf(c->b2, s->x0.left, s->s21.left);
        s->s21.left = msubf(c->a2, acc, s->s21.left);

        s->s12.left = s->s11.left;
        s->s11.left = 0;
        s->s22.left = s->s21.left;
        s->s21.left = 0;

        a[i].left = acc;
        
        acc = 0;
        acc  =  macf(c->b0, s->x0.right, s->s12.right);

        s->s11.right =  macf(c->b1, s->x0.right, s->s22.right);
        s->s11.right = msubf(c->a1, acc, s->s11.right);

        s->s21.right =  macf(c->b2, s->x0.right, s->s21.right);
        s->s21.right = msubf(c->a2, acc, s->s21.right);

        s->s12.right = s->s11.right;
        s->s11.right = 0;
        s->s22.right = s->s21.right;
        s->s21.right = 0;

        a[i].right = acc;

     

    }
   
   
    return 0;
}


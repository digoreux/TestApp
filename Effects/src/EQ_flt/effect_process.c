#include "effect_process.h"
#include "effect_control.h"
#include "fractional.h"

typedef struct stereo {
    flt left;
    flt right;
} stereo_t;


typedef struct states_s {
   stereo_t x0[10];
   stereo_t x1[10];
   stereo_t x2[10];
   stereo_t y0[10];
   stereo_t y1[10];
   stereo_t y2[10];
   stereo_t w0[10];
   stereo_t w1[10];
   stereo_t w2[10];
   stereo_t s11[10];
   stereo_t s12[10];
   stereo_t s21[10];
   stereo_t s22[10];
   
} states_t;

typedef struct coeffs_s {
    flt  b0[10];
    flt  b1[10];
    flt  b2[10];
    flt  a0[10];
    flt  a1[10];
    flt  a2[10];
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
    for(size_t i = 0; i < 10; i++)
    {
        s->x0[i].left = 0;
        s->x1[i].left = 0;
        s->x2[i].left = 0;
        s->y1[i].left = 0;
        s->y2[i].left = 0;

        s->x0[i].right = 0;
        s->x1[i].right = 0;
        s->x2[i].right = 0;
        s->y1[i].right = 0;
        s->y2[i].right = 0;

        s->w0[i].left = 0;
        s->w1[i].left = 0;
        s->w2[i].left = 0;
        s->w0[i].right = 0;
        s->w1[i].right = 0;
        s->w2[i].right = 0;

        s->s11[i].left = 0;
        s->s12[i].left = 0;
        s->s21[i].left = 0;
        s->s22[i].left = 0;
        s->s11[i].right = 0;
        s->s12[i].right = 0;
        s->s21[i].right = 0;
        s->s22[i].right = 0;
    }

    return 0;
}

int32_t effect_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{   
    flt acc = 0;
    flt w0 = 0;
    coeffs_t *c = (coeffs_t*)coeffs;
    states_t *s = (states_t*)states;
    stereo_t *a = (stereo_t*)audio;

    for (size_t i = 0; i < samples_count; i++)
    {   

        // Direct Form I
        // for(uint8_t j = 0; j < 10; j++)
        // {
        //     s->x0[j].left  = a[i].left;
        //     s->x0[j].right = a[i].right;

        //     acc = 0;
        //     acc =  macf(c->b0[j], s->x0[j].left, acc);
        //     acc =  macf(c->b1[j], s->x1[j].left, acc);
        //     acc =  macf(c->b2[j], s->x2[j].left, acc);
        //     acc = msubf(c->a1[j], s->y1[j].left, acc);
        //     acc = msubf(c->a2[j], s->y2[j].left, acc);

        //     a[i].left = acc;

        //     s->x2[j].left = s->x1[j].left;
        //     s->x1[j].left = s->x0[j].left;
        //     s->y2[j].left = s->y1[j].left;
        //     s->y1[j].left = acc;

        //     acc = 0;
        //     acc =  macf(c->b0[j], s->x0[j].right, acc);
        //     acc =  macf(c->b1[j], s->x1[j].right, acc);
        //     acc =  macf(c->b2[j], s->x2[j].right, acc);
        //     acc = msubf(c->a1[j], s->y1[j].right, acc);
        //     acc = msubf(c->a2[j], s->y2[j].right, acc);

        //     s->x2[j].right = s->x1[j].right;
        //     s->x1[j].right = s->x0[j].right;
        //     s->y2[j].right = s->y1[j].right;
        //     s->y1[j].right = acc;

        //     a[i].right = acc;
        // }


        // Direct Form II
        // for(uint8_t j = 0; j < 10; j++)
        // {
        //     s->x0[j].left  = a[i].left;
        //     s->x0[j].right = a[i].right;

        //     acc = 0;
        //     s->w0[j].left = msubf(c->a1[j], s->w1[j].left, s->x0[j].left);
        //     s->w0[j].left = msubf(c->a2[j], s->w2[j].left, s->w0[j].left);

        //     acc =  macf(c->b0[j], s->w0[j].left, acc);
        //     acc =  macf(c->b1[j], s->w1[j].left, acc);
        //     acc =  macf(c->b2[j], s->w2[j].left, acc);


        //     s->w2[j].left = s->w1[j].left;
        //     s->w1[j].left = s->w0[j].left;
            
        //     a[i].left = acc;

        //     acc = 0;
        //     s->w0[j].right = msubf(c->a1[j], s->w1[j].right, s->x0[j].right);
        //     s->w0[j].right = msubf(c->a2[j], s->w2[j].right, s->w0[j].right);

        //     acc =  macf(c->b0[j], s->w0[j].right, acc);
        //     acc =  macf(c->b1[j], s->w1[j].right, acc);
        //     acc =  macf(c->b2[j], s->w2[j].right, acc);

        //     s->w2[j].right = s->w1[j].right;
        //     s->w1[j].right = s->w0[j].right;

        //     a[i].right = acc;

        // }

        // Direct Form II Transposed (not working)
        for (uint8_t j = 0; j < 10; j++)
        {
            s->x0[j].left  = a[i].left;
            s->x0[j].right = a[i].right;

            s->y0[j].left  = 0;
            s->y0[j].left  =  macf(c->b0[j], s->x0[j].left, s->s12[j].left);

            s->s11[j].left =  macf(c->b1[j], s->x0[j].left, s->s22[j].left);
            s->s11[j].left = msubf(c->a1[j], s->y0[j].left, s->s11[j].left);

            s->s21[j].left =  macf(c->b2[j], s->x0[j].left, s->s21[j].left);
            s->s21[j].left = msubf(c->a2[j], s->y0[j].left, s->s21[j].left);

            s->s12[j].left = s->s11[j].left;
            s->s22[j].left = s->s21[j].left;

            a[i].left = s->y0[j].left;

            s->y0[j].right  = 0;
            s->y0[j].right  =  macf(c->b0[j], s->x0[j].right, s->s12[j].right);

            s->s21[j].right =  macf(c->b2[j], s->x0[j].right, s->s21[j].right);
            s->s21[j].right = msubf(c->a2[j], s->y0[j].right, s->s21[j].right);

            s->s11[j].right =  macf(c->b1[j], s->x0[j].right, s->s22[j].right);
            s->s11[j].right = msubf(c->a1[j], s->y0[j].right, s->s11[j].right);


            s->s12[j].right = s->s11[j].right;
            s->s22[j].right = s->s21[j].right;

            a[i].right = s->y0[j].right;

        }
        
    }
   
   
    return 0;
}


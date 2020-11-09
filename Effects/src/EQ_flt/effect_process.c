﻿#include "effect_process.h"
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
        s->y0[i].left = 0;
        s->y1[i].left = 0;
        s->y2[i].left = 0;

        s->x0[i].right = 0;
        s->x1[i].right = 0;
        s->x2[i].right = 0;
        s->y0[i].right = 0;
        s->y1[i].right = 0;
        s->y2[i].right = 0;

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
    coeffs_t *c = (coeffs_t*)coeffs;
    states_t *s = (states_t*)states;
    stereo_t *a = (stereo_t*)audio;

    for (size_t i = 0; i < samples_count; i++)
    {   
        for(size_t j = 0; j < 10; j++)
        {   
            if(c->a0[j] != 0)
            {

                s->x0[j].left  = a[i].left;
                s->x0[j].right = a[i].right;

                s->y0[j].left =  macf(c->b0[j], s->x0[j].left, s->x1[j].left);

                s->x1[j].left =  macf(c->b1[j], s->x0[j].left, s->x2[j].left);
                s->x1[j].left = msubf(c->a1[j], s->y0[j].left, s->x1[j].left);

                s->x2[j].left =  mulf(c->b2[j], s->x0[j].left);
                s->x2[j].left = msubf(c->a2[j], s->y0[j].left, s->x2[j].left);

                a[i].left = s->y0[j].left;


                s->y0[j].right =  macf(c->b0[j], s->x0[j].right, s->x1[j].right);

                s->x1[j].right =  macf(c->b1[j], s->x0[j].right, s->x2[j].right);
                s->x1[j].right = msubf(c->a1[j], s->y0[j].right, s->x1[j].right);

                s->x2[j].right =  mulf(c->b2[j], s->x0[j].right);
                s->x2[j].right = msubf(c->a2[j], s->y0[j].right, s->x2[j].right);

                a[i].right = s->y0[j].right;
            }
        }
    }
    return 0;
}
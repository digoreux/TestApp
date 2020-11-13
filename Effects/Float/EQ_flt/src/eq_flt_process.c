#include "eq_flt_process.h"


int32_t eq_process_get_sizes(
    size_t*     states_bytes)
{   
    *states_bytes = sizeof(eq_states_t);
    return 0;
}

int32_t eq_reset(
    void const* coeffs,
    void*       states)
{   

    eq_states_t * s = (eq_states_t*)states;
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

int32_t eq_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{   
    flt acc = 0;
    eq_coeffs_t *c = (eq_coeffs_t*)coeffs;
    eq_states_t *s = (eq_states_t*)states;
    eq_stereo_t *a = (eq_stereo_t*)audio;

    for (size_t i = 0; i < samples_count; i++)
    {   
        for(size_t j = 0; j < 10; j++)
        {   
            if(c->a0[j] != 0)
            {

                s->x0[j].left  = a[i].left;
                s->x0[j].right = a[i].right;

                // printf("INPUT:  %0.30f  * %0.30f \n", s->x0[j].left, c->b0[j]);
                // printf("OUTPUT: %0.30f\n", s->y0[j].left);
                // printf("STATE:  %10.30f\n\n", s->x1[j].left);

                s->y0[j].left =  macf(c->b0[j], s->x0[j].left, s->x1[j].left) * 8;

                s->x1[j].left =  macf(c->b1[j], s->x0[j].left, s->x2[j].left);
                s->x1[j].left = msubf(c->a1[j], s->y0[j].left, s->x1[j].left);

                s->x2[j].left =  mulf(c->b2[j], s->x0[j].left);
                s->x2[j].left = msubf(c->a2[j], s->y0[j].left, s->x2[j].left);

                a[i].left = s->y0[j].left;


                s->y0[j].right =  macf(c->b0[j], s->x0[j].right, s->x1[j].right) * 8;

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
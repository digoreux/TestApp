#include "cross_flt_process.h"

int32_t cross_process_get_sizes(
    size_t*     states_bytes)
{
    *states_bytes = sizeof(cross_states_t);
    return 0;
}

int32_t cross_reset(
    void const* coeffs,
    void*       states)
{
    cross_states_t * s = (cross_states_t*)states;

    s->xn.left  = 0;
    s->xn.right = 0;

    /* 2nd Order */

    for(int i = 0; i < 2; i++)
    {
        s->y1[i].left = 0;
        s->x2[i].left = 0;
        s->x3[i].left = 0;

        s->y1[i].right = 0;
        s->x2[i].right = 0;
        s->x3[i].right = 0;
    }

    /* 1st Order */

    for(int i = 0; i < 3; i++)
    {   
        s->y0[i].left = 0;
        s->x0[i].left = 0;
        s->x1[i].left = 0;

        s->y0[i].right = 0;
        s->x0[i].right = 0;
        s->x1[i].right = 0;
    }
    return 0;
}

int32_t cross_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{   
    cross_coeffs_t* c = (cross_coeffs_t*)coeffs;
    cross_states_t* s = (cross_states_t*)states;
    cross_stereo_t* a = (cross_stereo_t*)audio;
    flt b1 = 0, b2 = 0;
    for(uint32_t i = 0; i < samples_count; i++)
    {   

        /* 1st Order */
        s->xn.left = a[i].left;
        s->x0[0].left = msubf(c->k0, s->x1[0].left, s->xn.left); 
        s->y0[0].left =  macf(c->k0, s->x0[0].left, s->x1[0].left);
        s->x1[0].left = s->x0[0].left;

        /* 2nd Order */
        s->y1[0].left =  macf(c->k2, s->xn.left,    s->x2[0].left);
        s->x2[0].left =  macf(c->k1, s->xn.left,    s->x3[0].left);
        s->x2[0].left = msubf(c->k1, s->y1[0].left, s->x2[0].left);
        s->x3[0].left = msubf(c->k2, s->y1[0].left, s->xn.left);

        s->xn.left = (s->y0[0].left + s->y1[0].left) / 2;

        /* 1st Order */
        s->x0[1].left = msubf(c->k0, s->x1[1].left, s->xn.left); 
        s->y0[1].left =  macf(c->k0, s->x0[1].left, s->x1[1].left);
        s->x1[1].left = s->x0[1].left;

        /* 2nd Order */
        s->y1[1].left =  macf(c->k2, s->xn.left,    s->x2[1].left);
        s->x2[1].left =  macf(c->k1, s->xn.left,    s->x3[1].left);
        s->x2[1].left = msubf(c->k1, s->y1[1].left, s->x2[1].left);
        s->x3[1].left = msubf(c->k2, s->y1[1].left, s->xn.left);

        /* 1st Order Phase Compensation */
        s->x0[2].left = msubf(c->k0, s->x1[2].left, s->y1[0].left); 
        s->y0[2].left =  macf(c->k0, s->x0[2].left, s->x1[2].left);
        s->x1[2].left = s->x0[2].left;

        b1 = (s->y0[1].left + s->y1[1].left) / 2;
        b2 =  s->y0[2].left - b1;

        s->band1[i].left = b1;
        s->band2[i].left = b2;


        /* 1st Order */
        s->xn.right = a[i].right;
        s->x0[0].right = msubf(c->k0, s->x1[0].right, s->xn.right); 
        s->y0[0].right =  macf(c->k0, s->x0[0].right, s->x1[0].right);
        s->x1[0].right = s->x0[0].right;

        /* 2nd Order */
        s->y1[0].right =  macf(c->k2, s->xn.right,    s->x2[0].right);
        s->x2[0].right =  macf(c->k1, s->xn.right,    s->x3[0].right);
        s->x2[0].right = msubf(c->k1, s->y1[0].right, s->x2[0].right);
        s->x3[0].right = msubf(c->k2, s->y1[0].right, s->xn.right);

        s->xn.right = (s->y0[0].right + s->y1[0].right) / 2;

        /* 1st Order */
        s->x0[1].right = msubf(c->k0, s->x1[1].right, s->xn.right); 
        s->y0[1].right =  macf(c->k0, s->x0[1].right, s->x1[1].right);
        s->x1[1].right = s->x0[1].right;

        /* 2nd Order */
        s->y1[1].right =  macf(c->k2, s->xn.right,    s->x2[1].right);
        s->x2[1].right =  macf(c->k1, s->xn.right,    s->x3[1].right);
        s->x2[1].right = msubf(c->k1, s->y1[1].right, s->x2[1].right);
        s->x3[1].right = msubf(c->k2, s->y1[1].right, s->xn.right);

        /* 1st Order Phase Compensation */
        s->x0[2].right = msubf(c->k0, s->x1[2].right, s->y1[0].right); 
        s->y0[2].right =  macf(c->k0, s->x0[2].right, s->x1[2].right);
        s->x1[2].right = s->x0[2].right;

        b1 = (s->y0[1].right + s->y1[1].right) / 2;
        b2 =  s->y0[2].right - b1;

        s->band1[i].right = b1;
        s->band2[i].right = b2;
 
    }   
    return 0;
}


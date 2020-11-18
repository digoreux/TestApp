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

        s->my1[i].v = _mm_set_ps(s->y1[i].left, s->y1[i].left, 0.0f, 0.0f);
        s->mx2[i].v = _mm_set_ps(s->x2[i].left, s->x2[i].left, 0.0f, 0.0f);
        s->mx3[i].v = _mm_set_ps(s->x3[i].left, s->x3[i].left, 0.0f, 0.0f);

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
        s->my0[i].v = _mm_set_ps(s->y0[i].left, s->y0[i].left, 0.0f, 0.0f);
        s->mx0[i].v = _mm_set_ps(s->x0[i].left, s->x0[i].left, 0.0f, 0.0f);
        s->mx1[i].v = _mm_set_ps(s->x1[i].left, s->x1[i].left, 0.0f, 0.0f);

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
    stereo_t* band1,
    stereo_t* band2,
    size_t samples_count)
{   
    cross_coeffs_t* c = (cross_coeffs_t*)coeffs;
    cross_states_t* s = (cross_states_t*)states;
    stereo_t* a = (stereo_t*)audio;
    flt b1 = 0, b2 = 0;
    for(uint32_t i = 0; i < samples_count; i++)
    {   

        s->mxn.v = _mm_set_ps(a[i].left, a[i].right, 0.0f, 0.0f);

        /* 1st Order */
        s->mx0[0].v = _mm_fnmadd_ps(c->mk0.v, s->mx1[0].v, s->mxn.v);
        s->my0[0].v =  _mm_fmadd_ps(c->mk0.v, s->mx0[0].v, s->mx1[0].v);
        s->mx1[0].v = s->mx0[0].v;

        /* 2nd Order */
        s->my1[0].v =  _mm_fmadd_ps(c->mk2.v, s->mxn.v,    s->mx2[0].v);
        s->mx2[0].v =  _mm_fmadd_ps(c->mk1.v, s->mxn.v,    s->mx3[0].v);
        s->mx2[0].v = _mm_fnmadd_ps(c->mk1.v, s->my1[0].v, s->mx2[0].v);
        s->mx3[0].v = _mm_fnmadd_ps(c->mk2.v, s->my1[0].v, s->mxn.v);

        s->mxn.v = _mm_add_ps(s->my0[0].v, s->my1[0].v);
        s->mxn.v = _mm_mul_ps(s->mxn.v, c->m2.v);

        /* 1st Order */
        s->mx0[1].v = _mm_fnmadd_ps(c->mk0.v, s->mx1[1].v, s->mxn.v);
        s->my0[1].v =  _mm_fmadd_ps(c->mk0.v, s->mx0[1].v, s->mx1[1].v);
        s->mx1[1].v = s->mx0[1].v;

        /* 2nd Order */
        s->my1[1].v =  _mm_fmadd_ps(c->mk2.v, s->mxn.v,    s->mx2[1].v);
        s->mx2[1].v =  _mm_fmadd_ps(c->mk1.v, s->mxn.v,    s->mx3[1].v);
        s->mx2[1].v = _mm_fnmadd_ps(c->mk1.v, s->my1[1].v, s->mx2[1].v);
        s->mx3[1].v = _mm_fnmadd_ps(c->mk2.v, s->my1[1].v, s->mxn.v);

        /* 1st Order Phase Compensation */
        s->mx0[2].v = _mm_fnmadd_ps(c->mk0.v, s->mx1[2].v, s->my1[0].v);
        s->my0[2].v =  _mm_fmadd_ps(c->mk0.v, s->mx0[2].v, s->mx1[2].v);
        s->mx1[2].v = s->mx0[2].v;

        s->mxn.v = _mm_add_ps(s->my0[1].v, s->my1[1].v);
        s->mxn.v = _mm_mul_ps(s->mxn.v, c->m2.v);

        band1[i].left  = s->mxn.f[3];
        band1[i].right = s->mxn.f[2];

        s->mxn.v =  _mm_sub_ps(s->my0[2].v, s->mxn.v);

        band2[i].left  = s->mxn.f[3];
        band2[i].right = s->mxn.f[2];

        // s->xn.left = a[i].left;
        // s->x0[0].left = msubf(c->k0, s->x1[0].left, s->xn.left); 
        // s->y0[0].left =  macf(c->k0, s->x0[0].left, s->x1[0].left);
        // s->x1[0].left = s->x0[0].left;

        // /* 2nd Order */
        // s->y1[0].left =  macf(c->k2, s->xn.left,    s->x2[0].left);
        // s->x2[0].left =  macf(c->k1, s->xn.left,    s->x3[0].left);
        // s->x2[0].left = msubf(c->k1, s->y1[0].left, s->x2[0].left);
        // s->x3[0].left = msubf(c->k2, s->y1[0].left, s->xn.left);

        // s->xn.left = (s->y0[0].left + s->y1[0].left) / 2;

        // /* 1st Order */
        // s->x0[1].left = msubf(c->k0, s->x1[1].left, s->xn.left); 
        // s->y0[1].left =  macf(c->k0, s->x0[1].left, s->x1[1].left);
        // s->x1[1].left = s->x0[1].left;

        // /* 2nd Order */
        // s->y1[1].left =  macf(c->k2, s->xn.left,    s->x2[1].left);
        // s->x2[1].left =  macf(c->k1, s->xn.left,    s->x3[1].left);
        // s->x2[1].left = msubf(c->k1, s->y1[1].left, s->x2[1].left);
        // s->x3[1].left = msubf(c->k2, s->y1[1].left, s->xn.left);

        // /* 1st Order Phase Compensation */
        // s->x0[2].left = msubf(c->k0, s->x1[2].left, s->y1[0].left); 
        // s->y0[2].left =  macf(c->k0, s->x0[2].left, s->x1[2].left);
        // s->x1[2].left = s->x0[2].left;

        // b1 = (s->y0[1].left + s->y1[1].left) / 2;
        // b2 =  s->y0[2].left - b1;

        // band1[i].left = b1;
        // band2[i].left = b2;

        // /* 1st Order */
        // s->xn.right = a[i].right;
        // s->x0[0].right = msubf(c->k0, s->x1[0].right, s->xn.right); 
        // s->y0[0].right =  macf(c->k0, s->x0[0].right, s->x1[0].right);
        // s->x1[0].right = s->x0[0].right;

        // /* 2nd Order */
        // s->y1[0].right =  macf(c->k2, s->xn.right,    s->x2[0].right);
        // s->x2[0].right =  macf(c->k1, s->xn.right,    s->x3[0].right);
        // s->x2[0].right = msubf(c->k1, s->y1[0].right, s->x2[0].right);
        // s->x3[0].right = msubf(c->k2, s->y1[0].right, s->xn.right);

        // s->xn.right = (s->y0[0].right + s->y1[0].right) / 2;

        // /* 1st Order */
        // s->x0[1].right = msubf(c->k0, s->x1[1].right, s->xn.right); 
        // s->y0[1].right =  macf(c->k0, s->x0[1].right, s->x1[1].right);
        // s->x1[1].right = s->x0[1].right;

        // /* 2nd Order */
        // s->y1[1].right =  macf(c->k2, s->xn.right,    s->x2[1].right);
        // s->x2[1].right =  macf(c->k1, s->xn.right,    s->x3[1].right);
        // s->x2[1].right = msubf(c->k1, s->y1[1].right, s->x2[1].right);
        // s->x3[1].right = msubf(c->k2, s->y1[1].right, s->xn.right);

        // /* 1st Order Phase Compensation */
        // s->x0[2].right = msubf(c->k0, s->x1[2].right, s->y1[0].right); 
        // s->y0[2].right =  macf(c->k0, s->x0[2].right, s->x1[2].right);
        // s->x1[2].right = s->x0[2].right;

        // b1 = addf(s->y0[1].right, s->y1[1].right);
        // b1 = mulf(b1, 2);

        // b2 =  s->y0[2].right - b1;

        // band1[i].right = b1;
        // band2[i].right = b2;
 
    }   
    return 0;
}


#ifndef __CROSS_FLT_UTILS_H__
#define __CROSS_FLT_UTILS_H__

#include "cross_flt_structure.h"

static inline vector_t apff1(void * coeffs, void * states, stereo_t * b1, stereo_t * b2)
{
    cross4_coeffs_t* c = (cross4_coeffs_t*)coeffs;
    cross4_states_t* s = (cross4_states_t*)states;
    set_vals2(&s->xn[1], b1->left, b2->left, b1->right, b2->right);

    s->x0[1] = fnma2(c->k0, s->x1[1], s->xn[1]);
    s->y0[1] =  fma2(c->k0, s->x0[1], s->x1[1]);
    s->x1[1] = s->x0[1];

    return s->y0[1];
}

static inline vector_t apff2(void * coeffs, void * states, stereo_t * b1, stereo_t * b2)
{
    cross4_coeffs_t* c = (cross4_coeffs_t*)coeffs;
    cross4_states_t* s = (cross4_states_t*)states;

    set_vals2(&s->xn[2], b1->left, b2->left, b1->right, b2->right);

    s->y0[2] =   fnma2(c->k1, s->xn[2], s->x0[2]);
    s->x0[2] =    fma2(c->k2, s->xn[2], s->x1[2]);
    s->x0[2] =   fnma2(c->k2, s->y0[2], s->x0[2]);
    s->x1[2] =    fma2(c->k1, s->y0[2], s->xn[2]);


    return s->y0[2];
}

static inline vector_t apf1(void * coeffs, void * states, stereo_t * a)
{
    cross_coeffs_t* c = (cross_coeffs_t*)coeffs;
    cross_states_t* s = (cross_states_t*)states;
    set_vals(&s->xn, a->left, a->right);

    s->x0[0] = fnma2(c->k0, s->x1[0], s->xn);
    s->y0[0] =  fma2(c->k0, s->x0[0], s->x1[0]);
    s->x1[0] = s->x0[0];

    return s->y0[0];
}

static inline vector_t apf2(void * coeffs, void * states, stereo_t * a)
{
    cross_coeffs_t* c = (cross_coeffs_t*)coeffs;
    cross_states_t* s = (cross_states_t*)states;
    set_vals(&s->xn, a->left, a->right);

    s->y0[1] = fnma2(c->k1, s->xn,    s->x0[1]);
    s->x0[1] =  fma2(c->k2, s->xn,    s->x1[1]);
    s->x0[1] = fnma2(c->k2, s->y0[1], s->x0[1]);
    s->x1[1] =  fma2(c->k1, s->y0[1], s->xn);
    

    return s->y0[1];
}
static inline int32_t mix(void * audio, void * states, size_t samples_count) 
{
    stereo_t * a = (stereo_t *)audio;
    cross4_states_t* s = (cross4_states_t*)states;

    for(size_t i = 0; i < samples_count; i++) 
    {   
        a[i].left  = s->b1[i].left  + s->b2[i].left  + s->b3[i].left  + s->b4[i].left;
        a[i].right = s->b1[i].right + s->b2[i].right + s->b3[i].right + s->b4[i].right;
        //  a[i].left  = s->b1[i].left + s->b3[i].left;
        //  a[i].right = s->b2[i].left + s->b4[i].left;
    }
    return 0;
}

static inline int32_t mix2(void * audio, void * band1, void * band2, size_t samples_count) 
{
    stereo_t * a  = (stereo_t *)audio;
    stereo_t * b1 = (stereo_t *)band1;
    stereo_t * b2 = (stereo_t *)band2;

    for(size_t i = 0, j = 0; i < samples_count, j < FRAME_COUNT; i++, j++) 
    {
        // a[i].left  = b1[j].left  + b2[j].left;
        // a[i].right = b1[j].right + b2[j].right;
        a[i].left  = b1[j].left;
        a[i].right = b2[j].left;
    }
    return 0;
}

static inline int32_t phase_correction(void * coeffs, void * states, size_t samples_count)
{
    cross4_coeffs_t* c = (cross4_coeffs_t*)coeffs;
    cross4_states_t* s = (cross4_states_t*)states;

    for(size_t i = 0; i < samples_count; i++) 
    {   
        set_vals2(&s->xn[0], s->b3[i].left, s->b1[i].left, s->b3[i].right, s->b1[i].right);
    
        s->y0[0] =  fnma2(c->k1, s->xn[0], s->x0[0]);
        s->x0[0] =   fma2(c->k2, s->xn[0], s->x1[0]);
        s->x0[0] =  fnma2(c->k2, s->y0[0], s->x0[0]);
        s->x1[0] =   fma2(c->k1, s->y0[0], s->xn[0]);
    
        s->b3[i].left  = s->y0[0].val[3];
        s->b1[i].left  = s->y0[0].val[2];
        s->b3[i].right = s->y0[0].val[1];
        s->b1[i].right = s->y0[0].val[0];

    }
    return 0;
}

#endif 

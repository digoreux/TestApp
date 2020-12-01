#ifndef __CROSS_FXD_UTILS_H__
#define __CROSS_FXD_UTILS_H__

#include "cross_fxd_structure.h"

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

    s->y0[2] =  fma2(c->k2, s->xn[2], s->x0[2]);
    s->x0[2] =  fma2(c->k1, s->xn[2], s->x1[2]);
    s->x0[2] = fnma2(c->k1, s->y0[2], s->x0[2]);
    s->x1[2] = fnma2(c->k2, s->y0[2], s->xn[2]);

    return s->y0[2];
}

static inline stereo_t apf1(void * coeffs, void * states, stereo_t * a)
{
    cross_coeffs_t* c = (cross_coeffs_t*)coeffs;
    cross_states_t* s = (cross_states_t*)states;
    q63 acc = 0;
    s->x0[0].left = a->left;
    s->x0[0].left = a->right;
    acc = mac_q63(c->k0, right_shift_q31(s->x0[0].left, 1), acc);
    acc = msub_q63(c->k0, );
    

    return s->y0[0];
}

static inline vector_t apf2(void * coeffs, void * states, stereo_t * a)
{
    cross_coeffs_t* c = (cross_coeffs_t*)coeffs;
    cross_states_t* s = (cross_states_t*)states;
    set_vals(&s->xn, a->left, a->right);

    s->y0[1] =  fma2(c->k2, s->xn,    s->x0[1]);
    s->x0[1] =  fma2(c->k1, s->xn,    s->x1[1]);
    s->x0[1] = fnma2(c->k1, s->y0[1], s->x0[1]);
    s->x1[1] = fnma2(c->k2, s->y0[1], s->xn);

    return s->y0[1];
}

static inline int32_t mix(void * audio, void * states, size_t samples_count, size_t frames_count) 
{
    stereo_t * a = (stereo_t *)audio;
    cross4_states_t* s = (cross4_states_t*)states;
    uint32_t n = samples_count * frames_count; 

    for(size_t i = 0 + n, j = 0; i < samples_count + n, j < samples_count; i++, j++) 
    {   
        a[i].left  = s->b1[j].left  + s->b2[j].left  + s->b3[j].left  + s->b4[j].left;
        a[i].right = s->b1[j].right + s->b2[j].right + s->b3[j].right + s->b4[j].right;
        //  a[i].left  = s->b1[j].left + s->b3[j].left;
        //  a[i].right = s->b2[j].left + s->b4[j].left;
    }
    return 0;
}

static inline int32_t mix2(void * audio, void * band1, void * band2, size_t samples_count, size_t frames_count) 
{
    stereo_t * a  = (stereo_t *)audio;
    stereo_t * b1 = (stereo_t *)band1;
    stereo_t * b2 = (stereo_t *)band2;
    uint32_t n = samples_count * frames_count; 

    for(size_t i = 0 + n, j = 0; i < samples_count + n, j < samples_count; i++, j++) 
    {
        a[i].left  = b1[j].left  + b2[j].left;
        a[i].right = b1[j].right + b2[j].right;
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
    
        s->y0[0] =  fma2(c->k2, s->xn[0], s->x0[0]);
        s->x0[0] =  fma2(c->k1, s->xn[0], s->x1[0]);
        s->x0[0] = fnma2(c->k1, s->y0[0], s->x0[0]);
        s->x1[0] = fnma2(c->k2, s->y0[0], s->xn[0]);
    
        s->b3[i].left  = s->y0[0].val[3];
        s->b1[i].left  = s->y0[0].val[2];
        s->b3[i].right = s->y0[0].val[1];
        s->b1[i].right = s->y0[0].val[0];

    }
    return 0;
}

#endif 

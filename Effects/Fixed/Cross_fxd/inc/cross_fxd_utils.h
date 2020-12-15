#ifndef __CROSS_FXD_UTILS_H__
#define __CROSS_FXD_UTILS_H__

#define SCALE 1

#include "cross_fxd_structure.h"

static inline stereo_t apf1(void * coeffs, void * states, stereo_t * a)
{
    cross_coeffs_t* c = (cross_coeffs_t*)coeffs;
    cross_states_t* s = (cross_states_t*)states;

    q63 acc;
    s->x0[0].left  = rshift_q31(a->left,  SCALE);
    s->x0[0].right = rshift_q31(a->right, SCALE);

    acc = 0;
    acc =  mac_q63(c->k0, s->x0[0].left, acc);
    acc = msub_q63(c->k0, s->y1[0].left, acc);
    acc =  add_q63(acc, lshift_q63(s->x1[0].left, 31));

    acc = lshift_q63(acc, 1);

    s->x1[0].left = s->x0[0].left;
    s->y1[0].left = gethigh(acc);
    s->y0[0].left = lshift_q31(gethigh(acc), SCALE);

    acc = 0;
    acc =  mac_q63(c->k0, s->x0[0].right, acc);
    acc = msub_q63(c->k0, s->y1[0].right, acc);
    acc =  add_q63(acc, lshift_q63(s->x1[0].right, 31));

    acc = lshift_q63(acc, 1);

    s->x1[0].right = s->x0[0].right;
    s->y1[0].right = gethigh(acc);
    s->y0[0].right = lshift_q31(s->y1[0].right, SCALE);

    return s->y0[0];
}

static inline stereo_t apf2(void * coeffs, void * states, stereo_t * a)
{
    cross_coeffs_t* c = (cross_coeffs_t*)coeffs;
    cross_states_t* s = (cross_states_t*)states;
    stereo_t y, x;
    q63 acc;

    x.left  = rshift_q31(a->left,  SCALE);
    x.right = rshift_q31(a->right, SCALE);

    acc = 0;
    acc =  mac_q63(c->k2, x.left, acc);
    acc = msub_q63(c->k2, s->y1[1].left, acc);
    acc =  mac_q63(c->k1, s->x0[1].left, acc);
    acc = msub_q63(c->k1, s->y0[1].left, acc);
    acc =  add_q63(acc, lshift_q63(s->x1[1].left, 31));

    acc = lshift_q63(acc, 1);

    s->x1[1].left = s->x0[1].left;
    s->x0[1].left = x.left;

    s->y1[1].left = s->y0[1].left;
    s->y0[1].left = gethigh(acc);

    y.left = lshift_q31(gethigh(acc), SCALE);

    acc = 0;
    acc =  mac_q63(c->k2, x.right, acc);
    acc = msub_q63(c->k2, s->y1[1].right, acc);
    acc =  mac_q63(c->k1, s->x0[1].right, acc);
    acc = msub_q63(c->k1, s->y0[1].right, acc);
    acc =  add_q63(acc, lshift_q63(s->x1[1].right, 31));

    acc = lshift_q63(acc, 1);

    s->x1[1].right = s->x0[1].right;
    s->x0[1].right = x.right;

    s->y1[1].right = s->y0[1].right;
    s->y0[1].right = gethigh(acc);

    y.right = lshift_q31(gethigh(acc), SCALE);

    return y;
}

static inline int32_t mix(void * audio, void * states, size_t samples_count)
{
    cross4_states_t* s = (cross4_states_t*)states;
    stereo_t * a = (stereo_t *)audio;

    for(size_t i = 0; i < samples_count; i++)
    {
        // a[i].left  = s->b1[i].left  + s->b2[i].left  + s->b3[i].left  + s->b4[i].left;
        // a[i].right = s->b1[i].right + s->b2[i].right + s->b3[i].right + s->b4[i].right;
        // a[i].left  = s->b1[i].left + s->b3[i].left;
        // a[i].right = s->b2[i].left + s->b4[i].left;
    }
    return 0;
}

static inline int32_t mix2(void * audio, void * band1, void * band2, size_t samples_count)
{
    stereo_t * a  = (stereo_t *)audio;
    stereo_t * b1 = (stereo_t *)band1;
    stereo_t * b2 = (stereo_t *)band2;

    for(size_t i = 0; i < samples_count; i++)
    {
        // a[i].left  = b1[i].left  + b2[i].left;
        // a[i].right = b1[i].right + b2[i].right;

        a[i].left  = b1[i].left;
        a[i].right = b2[i].left;
    }
    return 0;
}

static inline void apff2(
    stereo_t*   b, 
    q31         k1,
    q31         k2,
    stereo_t*   x0,
    stereo_t*   x1,
    stereo_t*   y0,
    stereo_t*   y1)
{
    stereo_t x;
    q63 acc;

    x.left  = rshift_q31(b->left,  SCALE);
    x.right = rshift_q31(b->right, SCALE);

    acc = 0;
    acc =  mac_q63(k2, x.left, acc);
    acc = msub_q63(k2, y1->left, acc);
    acc =  mac_q63(k1, x0->left, acc);
    acc = msub_q63(k1, y0->left, acc);
    acc =  add_q63(acc, lshift_q63(x1->left, 31));

    acc = lshift_q63(acc, 1);

    x1->left = x0->left;
    x0->left = x.left;
    y1->left = y0->left;
    y0->left = gethigh(acc);

    b->left = lshift_q31(gethigh(acc), SCALE);

    acc = 0;
    acc =  mac_q63(k2, x.right, acc);
    acc = msub_q63(k2, y1->right, acc);
    acc =  mac_q63(k1, x0->right, acc);
    acc = msub_q63(k1, y0->right, acc);
    acc =  add_q63(acc, lshift_q63(x1->right, 31));

    acc = lshift_q63(acc, 1);

    x1->right = x0->right;
    x0->right = x.right;
    y1->right = y0->right;
    y0->right = gethigh(acc);

    b->right = lshift_q31(gethigh(acc), SCALE);

}

static inline int32_t phase_correction(
    void *      coeffs, 
    void *      states, 
    stereo_t *  band1, 
    stereo_t *  band2, 
    size_t      samples_count)
{
    cross4_coeffs_t* c = (cross4_coeffs_t*)coeffs;
    cross4_states_t* s = (cross4_states_t*)states;

    for(size_t i = 0; i < samples_count; i++)
    {   
        apff2(&band1[i], c->cross[2].k1, c->cross[2].k2, 
              &s->x0[0], &s->x1[0], &s->y0[0], &s->y1[0]);

        apff2(&band2[i], c->cross[0].k1, c->cross[0].k2, 
              &s->x0[1], &s->x1[1], &s->y0[1], &s->y1[1]);

    }
    return 0;
}

#endif


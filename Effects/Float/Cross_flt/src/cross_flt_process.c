#include "cross_flt_process.h"

static inline uint32_t apf1(void * coeffs, void * states, void * audio, void * band);
static inline uint32_t apf2(void * coeffs, void * states, void * audio, void * band);

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

    set_val(&s->xn, 0.0f);
    for(int i = 0; i < 2; i++)
    {   
        set_val(&s->y0[i], 0.0f);
        set_val(&s->x0[i], 0.0f);
        set_val(&s->x1[i], 0.0f);
    }

    return 0;
}

int32_t cross_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    stereo_t*   band1,
    stereo_t*   band2,
    size_t      samples_count,
    size_t      frames_count)
{   
    cross_coeffs_t* c = (cross_coeffs_t*)coeffs;
    cross_states_t* s = (cross_states_t*)states;
    stereo_t      * a = (stereo_t*)audio;

    uint32_t n = samples_count * frames_count;

    stereo_t b1, b2;
    
    for(uint32_t i = 0 + n, j = 0; i < samples_count + n, j < samples_count; i++, j++)
    {   
        apf1(c, s, &a[i], &b1);
        apf2(c, s, &a[i], &b2);
        band1[j].left  = (b2.left  + b1.left)  * 0.5f;
        band1[j].right = (b2.right + b1.right) * 0.5f;
        band2[j].left  = (b2.left  - b1.left)  * 0.5f;
        band2[j].right = (b2.right - b1.right) * 0.5f;
    }   
    return 0;

}

int32_t cross_process_frame(
    void const* coeffs,
    void*       states,
    void*       audio,
    stereo_t*   band1,
    stereo_t*   band2,
    size_t      samples_count,
    size_t      frames_count)
{   
    cross_coeffs_t* c = (cross_coeffs_t*)coeffs;
    cross_states_t* s = (cross_states_t*)states;
    stereo_t      * a = (stereo_t*)audio;

    uint32_t n = samples_count * frames_count;

    stereo_t b1, b2;
    
    for(uint32_t i = 0 + n, j = 0; i < samples_count + n, j < samples_count; i++, j++)
    {   
        apf1(c, s, &a[j], &b1);
        apf2(c, s, &a[j], &b2);
        band1[j].left  = (b2.left  + b1.left)  * 0.5f;
        band1[j].right = (b2.right + b1.right) * 0.5f;
        band2[j].left  = (b2.left  - b1.left)  * 0.5f;
        band2[j].right = (b2.right - b1.right) * 0.5f;
    }   
    return 0;

}

static inline uint32_t apf1(void * coeffs, void * states, stereo_t * a, stereo_t * b)
{
    cross_coeffs_t* c = (cross_coeffs_t*)coeffs;
    cross_states_t* s = (cross_states_t*)states;
    set_vals(&s->xn, a->left, a->right);

    s->x0[0] = fnma2(c->k0, s->x1[0], s->xn);
    s->y0[0] =  fma2(c->k0, s->x0[0], s->x1[0]);
    s->x1[0] = s->x0[0];

    b->left  = s->y0[0].val[3];
    b->right = s->y0[0].val[2];

    return 0;
}

static inline uint32_t apf2(void * coeffs, void * states, stereo_t * a, stereo_t * b)
{
    cross_coeffs_t* c = (cross_coeffs_t*)coeffs;
    cross_states_t* s = (cross_states_t*)states;
    set_vals(&s->xn, a->left, a->right);

    s->y0[1] =  fma2(c->k2, s->xn,    s->x0[1]);
    s->x0[1] =  fma2(c->k1, s->xn,    s->x1[1]);
    s->x0[1] = fnma2(c->k1, s->y0[1], s->x0[1]);
    s->x1[1] = fnma2(c->k2, s->y0[1], s->xn);

    b->left  = s->y0[1].val[3];
    b->right = s->y0[1].val[2];

    return 0;
}


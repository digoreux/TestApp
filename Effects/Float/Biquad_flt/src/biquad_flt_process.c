#include "biquad_flt_process.h"

int32_t bq_process_get_sizes(
    size_t*     states_bytes)
{   
    *states_bytes = sizeof(bq_states_t);
    return 0;
}

int32_t bq_reset(
    void const* coeffs,
    void*       states)
{   
    bq_states_t * s = (bq_states_t*)states;

    set_val(&s->x0, 0.0f);
    set_val(&s->x1, 0.0f);
    set_val(&s->x2, 0.0f);
    set_val(&s->y0, 0.0f);

    s->sx0.left = 0;
    s->sx1.left = 0;
    s->sx2.left = 0;
    s->sy0.left = 0;

    s->sx0.right = 0;
    s->sx1.right = 0;
    s->sx2.right = 0;
    s->sy0.right = 0;

    return 0;
}

inline int32_t bq_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count,
    size_t      frames_count)
{   
    bq_coeffs_t *c = (bq_coeffs_t*)coeffs;
    bq_states_t *s = (bq_states_t*)states;
    stereo_t   * a = (stereo_t *)audio;

    uint32_t n = samples_count * frames_count;

    if(!c->bypass)
    {
        for (size_t i = 0 + n; i < samples_count + n; i++)
        {    
            // set_vals(&s->x0, a[i].left, a[i].right);
            s->x0.val[3] = a[i].left;
            s->x0.val[2] = a[i].right;

            s->y0 = fma2(c->b0, s->x0, s->x1);

            s->x1 = fma2(c->b1, s->x0, s->x2);
            s->x1 = fma2(c->a1, s->y0, s->x1);

            s->x2 = mul2(c->b2, s->x0);
            s->x2 = fma2(c->a2, s->y0, s->x2);
            
            a[i].left  = s->y0.val[3];
            a[i].right = s->y0.val[2];

            // s->x0.vec = _mm_set_ps(a[i].left, a[i].right, 0.0f, 0.0f);
            // s->y0.vec = _mm_fmadd_ps(c->b0.vec, s->x0.vec, s->x1.vec);
            // s->x1.vec = _mm_fmadd_ps(c->b1.vec, s->x0.vec, s->x2.vec);
            // s->x1.vec = _mm_fmadd_ps(c->a1.vec, s->y0.vec, s->x1.vec);
            // s->x2.vec =   _mm_mul_ps(c->b2.vec, s->x0.vec);
            // s->x2.vec = _mm_fmadd_ps(c->a2.vec, s->y0.vec, s->x2.vec);
            // a[i].left  = s->y0.val[3];
            // a[i].right = s->y0.val[2];
            

            // s->sx0.left  = a[i].left;
            // s->sx0.right = a[i].right;
            // s->sy0.left = macf(c->sb0, s->sx0.left, s->sx1.left);
            // s->sx1.left = macf(c->sb1, s->sx0.left, s->sx2.left);
            // s->sx1.left = macf(c->sa1, s->sy0.left, s->sx1.left);
            // s->sx2.left = mulf(c->sb2, s->sx0.left);
            // s->sx2.left = macf(c->sa2, s->sy0.left, s->sx2.left);
            // s->sy0.right = macf(c->sb0, s->sx0.right, s->sx1.right);
            // s->sx1.right = macf(c->sb1, s->sx0.right, s->sx2.right);
            // s->sx1.right = macf(c->sa1, s->sy0.right, s->sx1.right);
            // s->sx2.right = mulf(c->sb2, s->sx0.right);
            // s->sx2.right = macf(c->sa2, s->sy0.right, s->sx2.right);
            // a[i].left  = s->sy0.left;
            // a[i].right = s->sy0.right;

        }
    }
    return 0;
}
#include "biquad_fxd_process.h"

#define  NORM 3
#define SCALE 1

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

    s->x0.left = 0;
    s->x1.left = 0;
    s->x2.left = 0;
    s->y1.left = 0;
    s->y2.left = 0;
    s->x0.right = 0;
    s->x1.right = 0;
    s->x2.right = 0;
    s->y1.right = 0;
    s->y2.right = 0;
    s->error.left  = 0;
    s->error.right = 0;

    return 0;
}

inline int32_t bq_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{   
    bq_coeffs_t *c = (bq_coeffs_t*)coeffs;
    bq_states_t *s = (bq_states_t*)states;
    stereo_t   * a = (stereo_t *)audio;
    q63 acc = 0;
    if(!c->bypass)
    {
        for (size_t i = 0 ; i < samples_count ; i++)
        {   
            if(c->a0 != 0)
            {     
                s->x0.left  = rshift_q31(a[i].left, SCALE);
                s->x0.right = rshift_q31(a[i].right, SCALE);

                acc =  0;
                acc =  add_q63(acc, s->error.left);
                acc =  mac_q63(c->b0, s->x0.left, acc);
                acc =  mac_q63(c->b1, s->x1.left, acc);
                acc =  mac_q63(c->b2, s->x2.left, acc);
                acc = msub_q63(c->a1, s->y1.left, acc);
                acc = msub_q63(c->a2, s->y2.left, acc);

                s->error.left = getlow(acc);
                acc = lshift_q63(acc, NORM);              
                a[i].left = lshift_q31(gethigh(acc), SCALE);

                s->x2.left = s->x1.left;
                s->x1.left = s->x0.left;
                s->y2.left = s->y1.left;
                s->y1.left = gethigh(acc); 

                acc = 0;
                acc =  add_q63(acc, s->error.right);
                acc =  mac_q63(c->b0, s->x0.right, acc);
                acc =  mac_q63(c->b1, s->x1.right, acc);
                acc =  mac_q63(c->b2, s->x2.right, acc);
                acc = msub_q63(c->a1, s->y1.right, acc);
                acc = msub_q63(c->a2, s->y2.right, acc);

                s->error.right = getlow(acc);
                acc = lshift_q63(acc, NORM);               
                a[i].right = lshift_q31(gethigh(acc), SCALE);;

                s->x2.right = s->x1.right;
                s->x1.right = s->x0.right;
                s->y2.right = s->y1.right;
                s->y1.right = gethigh(acc);
            }
        }
    }
    return 0;
}
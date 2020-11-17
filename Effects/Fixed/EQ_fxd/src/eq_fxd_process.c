#include "eq_fxd_control.h"
#include "eq_fxd_process.h"

#define  NORM 3
#define SCALE 1

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
        s->y1[i].left = 0;
        s->y2[i].left = 0;

        s->x0[i].right = 0;
        s->x1[i].right = 0;
        s->x2[i].right = 0;
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
    q63 acc = 0;
    eq_coeffs_t *c = (eq_coeffs_t*)coeffs;
    eq_states_t *s = (eq_states_t*)states;
    eq_stereo_t *a = (eq_stereo_t*)audio;

    if(!c->bypass)
    {
        for (size_t i = 0; i < samples_count; i++)
        {   
            for(size_t j = 0; j < 10; j++)  
            {   
                if(c->a0[j] != 0)
                {
                    s->x0[j].left  = right_shift_q31(a[i].left, SCALE);
                    s->x0[j].right = right_shift_q31(a[i].right, SCALE);

                    acc =  0;
                    acc =  add_q63(acc, s->error[j].left);
                    acc =  mac_q63(c->b0[j], s->x0[j].left, acc);
                    acc =  mac_q63(c->b1[j], s->x1[j].left, acc);
                    acc =  mac_q63(c->b2[j], s->x2[j].left, acc);
                    acc = msub_q63(c->a1[j], s->y1[j].left, acc);
                    acc = msub_q63(c->a2[j], s->y2[j].left, acc);

                    s->error[j].left = getlow(acc);
                    acc = left_shift_q63(acc, NORM);              
                    a[i].left = left_shift_q31(gethigh(acc), SCALE);

                    s->x2[j].left = s->x1[j].left;
                    s->x1[j].left = s->x0[j].left;
                    s->y2[j].left = s->y1[j].left;
                    s->y1[j].left = gethigh(acc); 

                    acc = 0;
                    acc =  add_q63(acc, s->error[j].right);
                    acc =  mac_q63(c->b0[j], s->x0[j].right, acc);
                    acc =  mac_q63(c->b1[j], s->x1[j].right, acc);
                    acc =  mac_q63(c->b2[j], s->x2[j].right, acc);
                    acc = msub_q63(c->a1[j], s->y1[j].right, acc);
                    acc = msub_q63(c->a2[j], s->y2[j].right, acc);

                    s->error[j].right = getlow(acc);
                    acc = left_shift_q63(acc, NORM);               
                    a[i].right = left_shift_q31(gethigh(acc), SCALE);;

                    s->x2[j].right = s->x1[j].right;
                    s->x1[j].right = s->x0[j].right;
                    s->y2[j].right = s->y1[j].right;
                    s->y1[j].right = gethigh(acc); 
                }
            }
        }
    }
    return 0;
}

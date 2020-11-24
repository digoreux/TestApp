#include "comp4_flt_process.h"
#include "cross4_flt_control.h"

int32_t comp4_process_get_sizes(
    size_t*     states_bytes)
{
    *states_bytes = sizeof(comp4_states_t);

    return 0;
}

int32_t comp4_reset(
    void const* coeffs,
    void*       states)
{
    comp4_states_t* s = (comp4_states_t*)states;
    comp4_coeffs_t* c = (comp4_coeffs_t*)coeffs;

    set_val(&s->gc, 0.0f);
    set_val(&s->gm, 0.0f);
    set_val(&s->gs0, 0.0f);
    set_val(&s->gs1, 0.0f);
    set_val(&s->env0, 0.0f);
    set_val(&s->env1, 0.0f);

    return 0;
}


int32_t comp4_process(
    void const* coeffs,
    void*       states,
    void*       bands,
    size_t      samples_count,
    size_t      frames_count)
{
    comp4_coeffs_t  * c = (comp4_coeffs_t*)coeffs;
    comp4_states_t  * s = (comp4_states_t*)states;
    cross4_states_t * b = (cross4_states_t *)bands;
    flt abs1, abs2, abs3, abs4;
    vector_t abs;
    if(!c->bypass)
    {
        for (size_t i = 0 ; i < samples_count; i++)
        {
            abs1 = fmaxf(fabsf(b->b1[i].left), fabsf(b->b1[i].right));  
            abs2 = fmaxf(fabsf(b->b2[i].left), fabsf(b->b2[i].right));  
            abs3 = fmaxf(fabsf(b->b3[i].left), fabsf(b->b3[i].right));  
            abs4 = fmaxf(fabsf(b->b4[i].left), fabsf(b->b4[i].right));  

            set_vals2(&abs, abs4, abs3, abs2, abs1);

            /* Envelope detector */

            vector_t less, aux1, aux2;

            less = cmpgt(abs, s->env1);

            if (!less.val[3])             
            {
                s->env0 = mul2(c->envA, s->env1);       
                aux1 = sub2(c->one, c->envA);   
                s->env0 = fma2(aux1, abs, s->env0);     
            }
            else
            {
                s->env0 = mul2(c->envR, s->env1);          
                aux1 = sub2(c->one, c->envR);   
                s->env0 = fma2(aux1, abs, s->env0);             
            }

            s->env1 = s->env0;

            /* Gain computer */

            less = cmpgt(c->thrsh, s->env0);

            if (!less.val[3])
            {   
                set_val(&s->gc, 1.0f);
            }
            else
            {   
                aux1.vec = _mm_div_ps(s->env0.vec, c->thrsh.vec);
                aux1.vec = _mm_div_ps(c->one.vec,  c->ratio.vec);

                // s->gc = powf(aux1, aux2);

                s->gc = mul2(s->gc, c->thrsh);
                s->gc = div2(s->gc, s->env0);
            }

            /* Gain smoothing */
            // s->gc <= s->gs1
            less = cmple(s->gc, s->gs1);
            if (0)
            {
                s->gs0 = mul2(c->gainA, s->gs1);   
                aux1 = sub2(c->one, c->gainA);
                s->gs0 = fma2(aux1, s->gc, s->gs0);   
            }
            else
            {   
                s->gs0 = mul2(c->gainR, s->gs1);   
                aux1 = sub2(c->one, c->gainR);
                s->gs0 = fma2(aux1, s->gc, s->gs0); 
            }
    
            s->gs1 = s->gs0;
            s->gm  = mul2(s->gs0, c->gainM);          
            
            // a[i].left  *= s->gm;
            // a[i].right *= s->gm;
            
        }
    }    

   
    return 0;
}


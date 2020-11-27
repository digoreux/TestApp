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
    set_val(&s->gs1, 1.0f);
    set_val(&s->env1, 0.0f);
    set_val(&s->env0, 0.0f);

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
    vector_t abs, m, L, R;
    if(!c->bypass[4])
    {
        for (size_t i = 0 ; i < samples_count; i++)
        {   
            set_vals2(&L, b->b4[i].left,
                          b->b3[i].left,
                          b->b2[i].left,
                          b->b1[i].left);

            set_vals2(&R, b->b4[i].right,
                          b->b3[i].right,
                          b->b2[i].right,
                          b->b1[i].right);

            abs = max2(absf2(L), absf2(R));
 
            /* Envelope detector */

            m = cmpgt(abs, s->env1);
            s->env0 = mul2(blendv(c->envR,  c->envA,  m), s->env1);       
            s->env0 = fma2(blendv(c->oenvR, c->oenvA, m), abs, s->env0);  
            s->env1 = s->env0;

            /* Gain computer */

            m = cmplt(s->env0, c->thrsh);
            s->gc = vpow(div2(s->env0, c->thrsh), c->oratio);
            s->gc = mul2(s->gc, c->thrsh);
            s->gc = div2(s->gc, s->env0);
            s->gc = blendv(s->gc, c->one, m);

            /* Gain smoothing */

            m = cmple(s->gc, s->gs1);
            s->gs0 = mul2(blendv(c->gainA,  c->gainR,  m), s->gs1);   
            s->gs0 = fma2(blendv(c->ogainA, c->ogainR, m), s->gc, s->gs0);   
            s->gs1 = s->gs0;

            s->gm  = mul2(s->gs0, c->gainM);          

            L = mul2(L, s->gm);
            R = mul2(R, s->gm);
            
            b->b4[i].left  = L.val[3];
            b->b3[i].left  = L.val[2];
            b->b2[i].left  = L.val[1];
            b->b1[i].left  = L.val[0];
            b->b4[i].right = R.val[3];
            b->b3[i].right = R.val[2];
            b->b2[i].right = R.val[1];
            b->b1[i].right = R.val[0];

            // b->b4[i].left  *= s->gm.val[3];
            // b->b4[i].right *= s->gm.val[3];
            // b->b3[i].left  *= s->gm.val[2];
            // b->b3[i].right *= s->gm.val[2];
            // b->b2[i].left  *= s->gm.val[1];
            // b->b2[i].right *= s->gm.val[1];
            // b->b1[i].left  *= s->gm.val[0];
            // b->b1[i].right *= s->gm.val[0];

        }
    }    
    return 0;
}


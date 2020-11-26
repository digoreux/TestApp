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
    set_val(&s->env0, 1.0f);

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
    vector_t abs, m, L, R, aux1, aux2;
    // printf("bypass: %d \n", c->bypass[4]);
    if(1)
    {
        for (size_t i = 0 ; i < samples_count; i++)
        {   
            set_vals2(&L, fabsf(b->b4[i].left),
                          fabsf(b->b3[i].left),
                          fabsf(b->b2[i].left),
                          fabsf(b->b1[i].left));

            set_vals2(&R, fabsf(b->b4[i].right),
                          fabsf(b->b3[i].right),
                          fabsf(b->b2[i].right),
                          fabsf(b->b1[i].right));

            abs = max2(L, R);
 
            /* Envelope detector */

            m = cmpgt(abs, s->env1);
            aux1 = blendv(c->envR, c->envA, m);
            aux2 = blendv(c->oenvR, c->oenvA, m);
            s->env0 = mul2(aux1, s->env1);       
            s->env0 = fma2(aux2, abs, s->env0);  
            s->env1 = s->env0;

            /* Gain computer */

            m = cmplt(s->env0, c->thrsh);
            s->gc = vpow(div2(s->env0, c->thrsh), c->oratio);
            s->gc = mul2(s->gc, c->thrsh);
            s->gc = div2(s->gc, s->env0);
            s->gc = blendv(s->gc, c->one, m);

            /* Gain smoothing */

            m = cmple(s->gc, s->gs1);
            aux1 = blendv(c->gainA, c->gainR, m);
            aux2 = blendv(c->ogainA, c->ogainR, m);
            s->gs0 = mul2(aux1, s->gs1);   
            s->gs0 = fma2(aux2, s->gc, s->gs0);   
            s->gs1 = s->gs0;

            s->gm  = mul2(s->gs0, c->gainM);          

            // L = mul2(L, s->gm);
            // R = mul2(R, s->gm);
            b->b4[i].left  *= s->gm.val[3];
            b->b4[i].right *= s->gm.val[3];
            b->b3[i].left  *= s->gm.val[2];
            b->b3[i].right *= s->gm.val[2];
            b->b2[i].left  *= s->gm.val[1];
            b->b2[i].right *= s->gm.val[1];
            b->b1[i].left  *= s->gm.val[0];
            b->b1[i].right *= s->gm.val[0];

            // b->b4[i].left = s->env0.val[3];
            // b->b3[i].left = s->env0.val[2];
            // b->b2[i].left = s->env0.val[1];
            // b->b1[i].left = s->env0.val[0];
            // b->b4[i].left  = s->gc.val[3];
            // b->b3[i].left  = s->gc.val[2];
            // b->b2[i].left  = s->gc.val[1];
            // b->b1[i].left  = s->gc.val[0];
            // b->b4[i].left  = s->gm.val[3];
            // b->b3[i].left  = s->gm.val[2];
            // b->b2[i].left  = s->gm.val[1];
            // b->b1[i].left  = s->gm.val[0];

        }
    }    
    return 0;
}


#include "eq.h"
 

    
#if CH == 2
void eq_process(vfloat *x, eq_coef_t *coef, eq_states_t *st){
    for(size_t i = 0; i < EQ_BAND; i++){
        biquad_process(x, &coef->biquad_coef[i], &st->biquad_states[i]);
    }
}
#elif CH == 4

#elif CH == 8
void eq_process(vfloat *x, eq_coef_t *coef, eq_states_t *st){
    vfloat y;
    for(size_t i = 0; i < 3; i++){

        y = vmac(coef->b0[i], *x, st->x1[i]);
        x->ch[2] = y.ch[0];
        x->ch[3] = y.ch[1];

        y = vmac(coef->b0[i], *x, st->x1[i]);
        x->ch[4] = y.ch[2];
        x->ch[5] = y.ch[3];

        y = vmac(coef->b0[i], *x, st->x1[i]);
        x->ch[6] = y.ch[4];
        x->ch[7] = y.ch[5];

        y = vmac(coef->b0[i], *x, st->x1[i]);

        st->x1[i] = vmac(coef->b1[i], *x, st->x2[i]);
        st->x1[i] = vmac(coef->a1[i],  y, st->x1[i]);

        st->x2[i] = vmul(coef->b2[i], *x);
        st->x2[i] = vmac(coef->a2[i],  y, st->x2[i]);

        x->ch[0] = y.ch[6];
        x->ch[1] = y.ch[7];
    }
    x->ch[0] = y.ch[2];
    x->ch[1] = y.ch[3];
}
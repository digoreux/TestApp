#include "cross_fxd.h"
#define ROOM1 1
#define ROOM2 1

static inline audio_type apf_fxd_1st(audio_type x, coef_type k, audio_type *st_y, audio_type *st_x);
static inline audio_type apf_fxd_2nd(audio_type x, 
                            coef_type k1, coef_type k2,
                            audio_type *st_y1, audio_type *st_x1,
                            audio_type *st_y2, audio_type *st_x2);

static band2_t cross2b_fxd(audio_type x, cross2b_fxd_states_t *st, cross_fxd_coef_t *coef);
static audio_type compes_fxd(audio_type x, co_fxd_states_t *st, cross_fxd_coef_t *coef);

band4_t cross4b_fxd(audio_type x, cross4b_fxd_states_t *st, cross_fxd_coef_t *coef){
    band4_t res;
    // printf("HERE");
    band2_t f1 ;
    f1 = cross2b_fxd(x, &st->f1, &coef[1]);
    f1.band1 = compes_fxd(f1.band1, &st->c_f2, &coef[2]);
    f1.band2 = compes_fxd(f1.band2, &st->c_f0, &coef[0]);
    
    res.low = cross2b_fxd(f1.band1, &st->f0, &coef[0]);
    res.high = cross2b_fxd(f1.band2, &st->f2, &coef[2]);
    
    // res.low = cross2b_fxd(x, &st->f1, &coef[1]);
    // res.high.band1 = 0;
    // res.high.band2 = 0;
    // res.low.band2 = 0;
    // res.low.band1 = compes_fxd(x, &st->c_f0, &coef[2]);
    return res;
}


static band2_t cross2b_fxd(audio_type x, cross2b_fxd_states_t *st, cross_fxd_coef_t *coef){
    band2_t res;
    audio_type tmpx = fxd_rshift(x, 1);
/*******************************************************/
    //second order
    audio_type y_2nd = apf_fxd_2nd(tmpx, coef->k1_2, coef->k2_2, &st->s_1[0], &st->s_1[1], &st->s_1[2], &st->s_1[3]);
/*******************************************************/
    //first order
    audio_type y_1st = apf_fxd_1st(tmpx,  coef->k1_1,  &st->f_1[0], &st->f_1[1]);
/*******************************************************/
//     //get low 
    res.band1 = fxd_add(y_2nd, y_1st);
    res.band1 = fxd_rshift(res.band1, 1);
/*******************************************************/
    //1st order compesation
    tmpx = apf_fxd_1st(y_2nd,  coef->k1_1,  &st->f_c[0], &st->f_c[1]);
    tmpx = fxd_lshift(tmpx, 1);
/*******************************************************/
    //second order
    y_2nd = apf_fxd_2nd(res.band1, coef->k1_2, coef->k2_2, &st->s_2[0], &st->s_2[1], &st->s_2[2], &st->s_2[3]);
/*******************************************************/
    //first order
    y_1st = apf_fxd_1st(res.band1,  coef->k1_1,  &st->f_2[0], &st->f_2[1]);

/*******************************************************/
    //get low 
    res.band1 = fxd_add(y_2nd, y_1st);
/*******************************************************/;
    res.band2 = fxd_sub(tmpx, res.band1);

    return res;
}

//compensation
static audio_type compes_fxd(audio_type x, co_fxd_states_t *st, cross_fxd_coef_t *coef){

    //second order
    x = apf_fxd_2nd(x, coef->k1_2, coef->k2_2, &st->s[0], &st->s[1], &st->s[2], &st->s[3]);
/*******************************************************/
    //first order
    x = apf_fxd_1st(x,  coef->k1_1,  &st->f[0], &st->f[1]);

/*******************************************************/
    return x;
}

static inline audio_type apf_fxd_1st(audio_type x, coef_type k, audio_type *st_y, audio_type *st_x){
    acum_type  acum = 0;
    acum = fxd_mac_m(acum, k, fxd_rshift(x, ROOM1));

    acum = fxd_msub_m(acum, k, *st_y);

    acum = fxd63_add(acum, fxd63_lshift(*st_x, COEF_FR - 1));

    *st_x = fxd_rshift(x, ROOM1);


    *st_y = (audio_type)saturation(fxd63_rshift(acum, COEF_FR - 1));


    return (audio_type)saturation(fxd63_rshift(acum, COEF_FR - ROOM1 - 1));

}

static inline audio_type apf_fxd_2nd(audio_type x, 
                            coef_type k1, coef_type k2,
                             audio_type *st_y1, audio_type *st_x1,
                             audio_type *st_y2, audio_type *st_x2){
    acum_type  acum = 0;
    acum = fxd_mac_m(acum, k2, fxd_rshift(x, ROOM2));

    acum = fxd_msub_m(acum, k2, *st_y2);
    
    acum = fxd_mac_m(acum, k1,  *st_x1);

    acum = fxd_msub_m(acum, k1, *st_y1);

    acum = fxd63_add(acum, fxd63_lshift(*st_x2, COEF_FR - ROOM2));

    *st_x2 = *st_x1;
    *st_x1 = fxd_rshift(x, ROOM2);

    *st_y2 = *st_y1;
    *st_y1 = (audio_type)saturation(fxd63_rshift(acum, (COEF_FR - ROOM2)));



    // (audio_type)acum;

    acum = fxd63_rshift(acum, (COEF_FR - ROOM1 - 1));
    acum = saturation(acum);

    return (audio_type)acum;
}

// bands_t crossover_flt(audio_type x, cross_flt_states_t *st, cross_flt_coef_t *coef){
//     bands_t res;
//     audio_type xh = 0.0; 
//     audio_type  apl_2nd = apf_flt_2nd(x,       &(st->apf_2nd[0]), &coef->apf_2nd[1]);
//     audio_type  apl_1st = apf_flt_1st(x,       &(st->apf_1st[0]), &coef->apf_1st[1]);
//     audio_type  l_f2 = flt_add(apl_2nd, apl_1st) * 0.5f;
//     audio_type h_f2 = 0.0f;

// xh = apl_2nd;
// xh = apf_flt_1st(xh,       &(st->apf_1st[1]), &coef->apf_1st[1]);

//         apl_2nd = apf_flt_2nd(l_f2, &(st->apf_2nd[1]), &coef->apf_2nd[1]);
//         apl_1st = apf_flt_1st(l_f2, &(st->apf_1st[2]), &coef->apf_1st[1]);

//     l_f2 = flt_add(apl_2nd, apl_1st) * 0.5f;
//     h_f2 = xh - l_f2;

// //compensation f2

// l_f2 = apf_flt_2nd(l_f2, &(st->apf_2nd[7]), &coef->apf_2nd[2]);
// l_f2 = apf_flt_1st(l_f2, &(st->apf_1st[10]), &coef->apf_1st[2]);

// /****************************************************************/
    
//     apl_2nd = apf_flt_2nd(l_f2, &(st->apf_2nd[2]), &coef->apf_2nd[0]);   
//     apl_1st = apf_flt_1st(l_f2, &(st->apf_1st[3]), &coef->apf_1st[0]);
// xh = apl_2nd;
// xh = apf_flt_1st(xh,       &(st->apf_1st[4]), &coef->apf_1st[0]);
//     res.band1 = flt_add(apl_2nd, apl_1st) * 0.5f;

//         apl_2nd = apf_flt_2nd(res.band1, &(st->apf_2nd[3]), &coef->apf_2nd[0]);
//         apl_1st = apf_flt_1st(res.band1, &(st->apf_1st[5]), &coef->apf_1st[0]);

//     res.band1 = flt_add(apl_2nd, apl_1st) * 0.5f;
//     res.band2 = xh - res.band1;

// //compensation f0

// h_f2 = apf_flt_2nd(h_f2, &(st->apf_2nd[4]), &coef->apf_2nd[0]);
// h_f2 = apf_flt_1st(h_f2, &(st->apf_1st[6]), &coef->apf_1st[0]);

// /****************************************************************/

//     apl_2nd = apf_flt_2nd(h_f2, &(st->apf_2nd[5]), &coef->apf_2nd[2]);   
//     apl_1st = apf_flt_1st(h_f2, &(st->apf_1st[7]), &coef->apf_1st[2]);
// xh = apl_2nd;
// xh = apf_flt_1st(xh,       &(st->apf_1st[8]), &coef->apf_1st[2]);
//     res.band3 = flt_add(apl_2nd, apl_1st) * 0.5f;

//         apl_2nd = apf_flt_2nd(res.band3, &(st->apf_2nd[6]), &coef->apf_2nd[2]);
//         apl_1st = apf_flt_1st(res.band3, &(st->apf_1st[9]), &coef->apf_1st[2]);

//     res.band3 = flt_add(apl_2nd, apl_1st) * 0.5f;
//     res.band4 = xh - res.band3;

//     return res;
// }

// //Direct form 2
// static audio_type apf_flt_1st(audio_type x, apf_states_1st_t *st, apf_coef_1st_t *coef){
//     audio_type xh = flt_msub(x, coef->k1, st->xh[0]);
//     audio_type y =  flt_mac(st->xh[0], coef->k1, xh);

//     st->xh[0] = xh;
//     return y;
// }

// //Direct form 2 Transposed
// static audio_type apf_flt_2nd(audio_type x, apf_states_2nd_t *st, apf_coef_2nd_t *coef){
//     audio_type  y = flt_mac(st->xh[0], coef->k2, x);
    
//     st->xh[0] = flt_mac(st->xh[1], coef->k1, x);
//     st->xh[0] = flt_msub(st->xh[0], coef->k1, y);

//     st->xh[1] = flt_msub(x, coef->k2, y);

//     return y;
// }
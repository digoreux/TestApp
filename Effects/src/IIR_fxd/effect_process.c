#include "effect_process.h"
#include "effect_control.h"
#include "fractional.h"

typedef struct stereo_s {
    q31 left;
    q31 right;
} stereo_t;

typedef struct stereo_acc_s {
    q63 left;
    q63 right;
} stereo_acc_t;

typedef struct states_s {
   stereo_t x0;
   stereo_t x1;
   stereo_t x2;
   stereo_t y1;
   stereo_t y2;
   stereo_t w0;
   stereo_t w1;
   stereo_t w2;
   stereo_t s11;
   stereo_t s12;
   stereo_t s21;
   stereo_t s22;
   stereo_t error;
} states_t;

typedef struct coeffs_s {
    q31 b0;
    q31 b1;
    q31 b2;
    q31 a0;
    q31 a1;
    q31 a2;
} coeffs_t;

int32_t effect_process_get_sizes(
    size_t*     states_bytes)
{   
    *states_bytes = sizeof(states_t);
    return 0;
}

int32_t effect_reset(
    void const* coeffs,
    void*       states)
{   

    states_t * s = (states_t*)states;
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

    s->s11.left = 0;
    s->s12.left = 0;
    s->s21.left = 0;
    s->s22.left = 0;
 
    s->s11.right = 0;
    s->s12.right = 0;
    s->s21.right = 0;
    s->s22.right = 0;   

    s->w0.left = 0;
    s->w1.left = 0;
    s->w2.left = 0;

    s->w0.right = 0;
    s->w1.right = 0;
    s->w2.right = 0; 

    return 0;
}

int32_t effect_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{   
    q63 acc = 0;
    q63 w0 = 0;
    q31 error = 0;
    coeffs_t *c = (coeffs_t*)coeffs;
    states_t *s = (states_t*)states;
    stereo_t *a = (stereo_t*)audio;
    for (size_t i = 0; i < samples_count; i++)
    {   
    
        s->x0.left  = a[i].left;
        s->x0.right = a[i].right;

        /* warning mul_q63 temporarily does not shift left */

        acc = left_shift_q63((q63)s->x0.left, 27);          //Q31 << 27 = Q58
        acc = msub_q31(c->a1, s->w1.left, acc);             //Q30 * Q28 = Q58
        acc = msub_q31(c->a2, s->w2.left, acc);             //Q30 * Q28 = Q58
        
        // s->w0.left = gethigh(left_shift_q63(acc, 2));       //Q28
        s->w0.left = (q31)(acc >> 30);                         //Q58 >> 30 = Q28

        acc =  mul_q63(c->b0, s->w0.left);                  //Q30 * Q28 = Q58
        acc =  mac_q31(c->b1, s->w1.left,  acc);            //Q30 * Q28 = Q58
        acc =  mac_q31(c->b2, s->w2.left,  acc);            //Q30 * Q28 = Q58
 
        s->w2.left  = s->w1.left;                             //Q28
        s->w1.left  = s->w0.left;                             //Q28

        // acc = left_shift_q63(acc, 5);
        a[i].left = (q31)(acc >> 27);                       //Q58 >> 27 = Q31


        w0 = (s->x0.right << 32);
        w0 = msub_q31(c->a1, s->w1.right, w0);
        w0 = msub_q31(c->a2, s->w2.right, w0);

        acc =  0;
        acc =  mac_q31(c->b0, gethigh(w0),  acc);
        acc =  mac_q31(c->b1, s->w1.right,  acc);
        acc =  mac_q31(c->b2, s->w2.right,  acc);
 
        s->w2.right  = s->w1.right;
        s->w1.right  = gethigh(w0);

        acc = left_shift_q63(acc, 1);
        a[i].right = gethigh(acc);
         
    }
    return 0;
}


        /*
            DIRECT FORM I (done)
        */

        // s->x0.left  = a[i].left;
        // s->x0.right = a[i].right;

        // acc = 0;
        // acc = add_q63(acc, s->error.left);
        // acc = mac_q31(c->b0, s->x0.left, acc);
        // acc = mac_q31(c->b1, s->x1.left, acc);
        // acc = mac_q31(c->b2, s->x2.left, acc);
        // acc = msub_q31(c->a1, s->y1.left, acc); 
        // acc = msub_q31(c->a2, s->y2.left, acc);

        // s->error.left = getlow(acc);
        // acc = left_shift_q63(acc, 3);           // compensate Q30 coeffs
        // a[i].left = gethigh(acc);  


        // s->x2.left = s->x1.left;
        // s->x1.left = s->x0.left;
        // s->y2.left = s->y1.left;
        // s->y1.left = gethigh(acc);

        // acc = 0;
        // acc = add_q63(acc, s->error.right);
        // acc = mac_q31(c->b0, s->x0.right, acc);
        // acc = mac_q31(c->b1, s->x1.right, acc);
        // acc = mac_q31(c->b2, s->x2.right, acc);
        // acc = msub_q31(c->a1, s->y1.right, acc);
        // acc = msub_q31(c->a2, s->y2.right, acc);

        // s->error.right = getlow(acc);
        // acc = left_shift_q63(acc, 3);           
        // a[i].right = gethigh(acc);  
    
        // s->x2.right = s->x1.right;
        // s->x1.right = s->x0.right;
        // s->y2.right = s->y1.right;
        // s->y1.right = gethigh(acc);

        /*
            DIRECT FORM II
        */
        // s->x0.left  = a[i].left;
        // s->x0.right = a[i].right;

        // acc = (s->x0.left << 25);
        // acc = msub_q31(c->a1, s->w1.left, acc);
        // acc = msub_q31(c->a2, s->w2.left, acc);
        
        // // s->w0.left = gethigh(left_shift_q63(acc, 1));
        // // s->w0.left = (q31)(acc >> 29);
        // s->w0.left = gethigh(acc);

        // acc =  mul_q63(c->b0, s->w0.left);
        // acc =  mac_q31(c->b1, s->w1.left,  acc);
        // acc =  mac_q31(c->b2, s->w2.left,  acc);
 
        // s->w2.left  = s->w1.left;
        // s->w1.left  = s->w0.left;

        // acc = left_shift_q63(acc, 3);
        // a[i].left = gethigh(acc);


        // w0 = (s->x0.right << 32);
        // w0 = msub_q31(c->a1, s->w1.right, w0);
        // w0 = msub_q31(c->a2, s->w2.right, w0);

        // acc =  0;
        // acc =  mac_q31(c->b0, gethigh(w0),  acc);
        // acc =  mac_q31(c->b1, s->w1.right,  acc);
        // acc =  mac_q31(c->b2, s->w2.right,  acc);
 
        // s->w2.right  = s->w1.right;
        // s->w1.right  = gethigh(w0);

        // acc = left_shift_q63(acc, 1);
        // a[i].right = gethigh(acc);
         

        /* 
             DIRECT FORM I TRANSPOSED
        */

        // s->x0.left  = add_q31(a[i].left,  s->s21.left);
        // s->x0.right = add_q31(a[i].right, s->s21.right);

        // /* Output */
        // acc = 0;
        // acc = mac_q31(c->b0, s->x0.left, (left_shift_q63(s->s11.left, 31)));
        // a[i].left = gethigh(left_shift_q63(acc, 1));

        // /* Feedforward state */
        // acc = mac_q31(c->b1, s->x0.left, (left_shift_q63(s->s12.left, 31)));
        // s->s11.left = gethigh(left_shift_q63(acc, 1));
        // acc = mul_q63(c->b2, s->x0.left);
        // s->s12.left = gethigh(left_shift_q63(acc, 1));          

        // /* Feedback state */
        // acc = mac_q31(neg_q31(c->a1), s->x0.left, (left_shift_q63(s->s22.left, 31)));
        // s->s21.left = gethigh(left_shift_q63(acc, 1)); 

        // acc = mul_q63(neg_q31(c->a2), s->x0.left);
        // s->s22.left = gethigh(left_shift_q63(acc, 1));
        

        //                  /*  RIGHT CHANNEL */

        // /* Output */
        // acc = 0;
        // acc = mac_q31(c->b0, s->x0.right, (left_shift_q63(s->s11.right, 31)));
        // a[i].right = gethigh(left_shift_q63(acc, 1));


        // /* Feedforward state */
        // acc = mac_q31(c->b1, s->x0.right, (left_shift_q63(s->s12.right, 31)));
        // s->s11.right = gethigh(left_shift_q63(acc, 1));

        // acc = mul_q63(c->b2, s->x0.right);
        // s->s12.right = gethigh(left_shift_q63(acc, 1));


        // /* Feedback state */
        // acc = mac_q31(neg_q31(c->a1), s->x0.right, (left_shift_q63(s->s22.right, 31)));
        // s->s21.right = gethigh(left_shift_q63(acc, 1));

        // acc = mul_q63(neg_q31(c->a2), s->x0.right);
        // s->s22.right = gethigh(left_shift_q63(acc, 1));

        /* 
             DIRECT FORM II TRANSPOSED (done)
        */    

        // s->x0.left  = a[i].left;
        // s->x0.right = a[i].right;

        // acc = (s->w1.left << 31);
        // acc = mac_q31(c->b0, s->x0.left, acc);

        // a[i].left = gethigh(left_shift_q63(acc, 1));

        // acc = (s->w2.left << 31);
        // acc =  mac_q31(c->b1, s->x0.left, acc);
        // acc = msub_q31(c->a1, a[i].left,  acc);
        // s->w1.left = gethigh(left_shift_q63(acc, 1));

        // acc =  mul_q63(c->b2, s->x0.left);
        // acc = msub_q31(c->a2, a[i].left,  acc);
        // s->w2.left = gethigh(left_shift_q63(acc, 1));


        // acc = (s->w1.left << 31);
        // acc = mac_q31(c->b0, s->x0.left, acc);

        // a[i].left = gethigh(left_shift_q63(acc, 1));

        // acc = (s->w2.left << 31);
        // acc =  mac_q31(c->b1, s->x0.left, acc);
        // acc = msub_q31(c->a1, a[i].left,  acc);
        // s->w1.left = gethigh(left_shift_q63(acc, 1));

        // acc =  mul_q63(c->b2, s->x0.left);
        // acc = msub_q31(c->a2, a[i].left,  acc);
        // s->w2.left = gethigh(left_shift_q63(acc, 1));

     
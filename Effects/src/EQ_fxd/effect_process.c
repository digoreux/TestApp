#include "effect_process.h"
#include "effect_control.h"
#include "fractional.h"

#define K 10
#define IN_SHIFT  (20 - K)         // Q31 << 20 = Q51 (input to acc)
#define OUT_SHIFT (12 + K)         // Q51 << 12 = Q63 >> 32 = Q31 (output)
#define SCALE 3                    // Q51 <<  1 = Q52 >> 32 = Q20 (states from acc)

#define T_K K
#define T_IN_SHIFT  (9 + T_K)             // Q31 >> 11 = Q20 (input to acc)
#define T_OUT_SHIFT (12 + T_K)            // Q51 << 12 = Q63 >> 32 = Q31 (output)
#define T_SHIFT      29                   // Q20 << 31 = Q51 (states to acc)

typedef struct stereo {
    q31 left;
    q31 right;
} stereo_t;

typedef struct states_s {
   stereo_t x0[10];
   stereo_t x1[10];
   stereo_t x2[10];
   stereo_t y0[10];
   stereo_t y1[10];
   stereo_t y2[10];
   stereo_t error[10];
} states_t;

typedef struct coeffs_s {
    q31 form;
    q31 b0[10];
    q31 b1[10];
    q31 b2[10];
    q31 a0[10];
    q31 a1[10];
    q31 a2[10];
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
    for(size_t i = 0; i < 10; i++)
    {
        s->x0[i].left = 0;
        s->x1[i].left = 0;
        s->x2[i].left = 0;
        s->y0[i].left = 0;
        s->y1[i].left = 0;
        s->y2[i].left = 0;
        s->error[i].left = 0;

        s->x0[i].right = 0;
        s->x1[i].right = 0;
        s->x2[i].right = 0;
        s->y0[i].right = 0;
        s->y1[i].right = 0;
        s->y2[i].right = 0;
        s->error[i].right = 0;

    }

    return 0;
}

int32_t effect_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{   
    q63 acc = 0;
    coeffs_t *c = (coeffs_t*)coeffs;
    states_t *s = (states_t*)states;
    stereo_t *a = (stereo_t*)audio;

    switch(c->form) 
    {
    case 0:  /* DIRECT FORM I */
        for (size_t i = 0; i < samples_count; i++)
        {   
            for(size_t j = 0; j < 10; j++)
            {   
                if(c->a0[j] != 0)
                {
                    s->x0[j].left  = a[i].left;
                    s->x0[j].right = a[i].right;

                    acc =  0;
                    acc =  add_q63(acc, s->error[j].left);
                    acc =  mac_q31(c->b0[j], s->x0[j].left, acc);
                    acc =  mac_q31(c->b1[j], s->x1[j].left, acc);
                    acc =  mac_q31(c->b2[j], s->x2[j].left, acc);
                    acc = msub_q31(c->a1[j], s->y1[j].left, acc);
                    acc = msub_q31(c->a2[j], s->y2[j].left, acc);

                    s->error[j].left = getlow(acc);
                    acc = left_shift_q63(acc, 3);               // compensate Q30 coeffs
                    a[i].left = gethigh(acc);

                    s->x2[j].left = s->x1[j].left;
                    s->x1[j].left = s->x0[j].left;
                    s->y2[j].left = s->y1[j].left;
                    s->y1[j].left = gethigh(acc); 

                    acc = 0;
                    acc =  add_q63(acc, s->error[j].right);
                    acc =  mac_q31(c->b0[j], s->x0[j].right, acc);
                    acc =  mac_q31(c->b1[j], s->x1[j].right, acc);
                    acc =  mac_q31(c->b2[j], s->x2[j].right, acc);
                    acc = msub_q31(c->a1[j], s->y1[j].right, acc);
                    acc = msub_q31(c->a2[j], s->y2[j].right, acc);

                    s->error[j].right = getlow(acc);
                    acc = left_shift_q63(acc, 3);               // compensate Q30 coeffs
                    a[i].right = gethigh(acc);

                    s->x2[j].right = s->x1[j].right;
                    s->x1[j].right = s->x0[j].right;
                    s->y2[j].right = s->y1[j].right;
                    s->y1[j].right = gethigh(acc); 
                } 
            }
        }
        break;
    case 1:  /* DIRECT FORM II */
        for (size_t i = 0; i < samples_count; i++)
        {      
            for(uint8_t j = 0; j < 10; j++)
            {
                if(c->a0[j] != 0) 
                {
                    s->x0[j].left  = a[i].left;
                    s->x0[j].right = a[i].right;

                    /* Feedback state */
                    acc = left_shift_q63((q63)s->x0[j].left, IN_SHIFT);         
                    acc = msub_q31(c->a1[j], s->x1[j].left, acc);            
                    acc = msub_q31(c->a2[j], s->x2[j].left, acc);            

                    s->x0[j].left = gethigh(left_shift_q63(acc, SCALE));      

                    /* Feedforward state */
                    acc = s->error[j].left;
                    acc = mul_q63(c->b0[j], s->x0[j].left);                  
                    acc = mac_q31(c->b1[j], s->x1[j].left,  acc);            
                    acc = mac_q31(c->b2[j], s->x2[j].left,  acc);            

                    s->x2[j].left = s->x1[j].left;                          
                    s->x1[j].left = s->x0[j].left;                          

                    /* Output */
                    acc = left_shift_q63(acc, OUT_SHIFT);                     
                    s->error[j].left = getlow(acc); 
                    a[i].left = gethigh(acc);                          

                    /*  RIGHT CHANNEL */

                    /* Feedback state */
                    acc = left_shift_q63((q63)s->x0[j].right, IN_SHIFT);         
                    acc = msub_q31(c->a1[j], s->x1[j].right, acc);            
                    acc = msub_q31(c->a2[j], s->x2[j].right, acc);            

                    s->x0[j].right = gethigh(left_shift_q63(acc, SCALE));      

                    /* Feedforward state */
                    acc = s->error[j].right;
                    acc = mul_q63(c->b0[j], s->x0[j].right);                  
                    acc = mac_q31(c->b1[j], s->x1[j].right,  acc);            
                    acc = mac_q31(c->b2[j], s->x2[j].right,  acc);            

                    s->x2[j].right = s->x1[j].right;                          
                    s->x1[j].right = s->x0[j].right;                          

                    /* Output */
                    acc = left_shift_q63(acc, OUT_SHIFT);                     
                    s->error[j].right = getlow(acc);
                    a[i].right = gethigh(acc);                   
                }
            }       
        }
        break;
    case 2:  /* DIRECT FORM I TRANSPOSED */
        for (size_t i = 0; i < samples_count; i++)
        {   
            for(uint8_t j = 0; j < 10; j++)
            {   
                if(c->a0[j] != 0) 
                {
                    s->x0[j].left  = add_q31(right_shift_q31(a[i].left,  T_IN_SHIFT), s->y1[j].left);      
                    s->x0[j].right = add_q31(right_shift_q31(a[i].right, T_IN_SHIFT), s->y1[j].right);    

                    /* Output */
                    acc = s->error[j].left;
                    acc = left_shift_q63(s->x1[j].left, T_SHIFT);                        
                    acc = mac_q31(c->b0[j], s->x0[j].left, acc);                       

                    s->error[j].left = getlow(acc);
                    acc = left_shift_q63(acc, T_OUT_SHIFT);                               
                    a[i].left = gethigh(acc);                                    

                    /* Feedforward state */
                    acc = left_shift_q63(s->x2[j].left, T_SHIFT);                        
                    acc = mac_q31(c->b1[j], s->x0[j].left, acc);                       
                    s->x1[j].left = gethigh(left_shift_q63(acc, SCALE));                

                    acc = mul_q63(c->b2[j], s->x0[j].left);                            
                    s->x2[j].left = gethigh(left_shift_q63(acc, SCALE));                

                    /* Feedback state */
                    acc = left_shift_q63(s->y2[j].left, T_SHIFT);
                    acc = mac_q31(neg_q31(c->a1[j]), s->x0[j].left, acc);
                    s->y1[j].left = gethigh(left_shift_q63(acc, SCALE)); 

                    acc = mul_q63(neg_q31(c->a2[j]), s->x0[j].left);
                    s->y2[j].left = gethigh(left_shift_q63(acc, SCALE));


                    /* Output */
                    // acc = s->error[j].right;
                    acc = left_shift_q63(s->x1[j].right, T_SHIFT);                        
                    acc = mac_q31(c->b0[j], s->x0[j].right, acc);                       

                    s->error[j].right= getlow(acc);
                    acc = left_shift_q63(acc, T_OUT_SHIFT);                               
                    a[i].right= gethigh(acc);                                    

                    /* Feedforward state */
                    acc = left_shift_q63(s->x2[j].right, T_SHIFT);                        
                    acc = mac_q31(c->b1[j], s->x0[j].right, acc);                       
                    s->x1[j].right= gethigh(left_shift_q63(acc, SCALE));                

                    acc = mul_q63(c->b2[j], s->x0[j].right);                            
                    s->x2[j].right= gethigh(left_shift_q63(acc, SCALE));                

                    /* Feedback state */
                    acc = left_shift_q63(s->y2[j].right, T_SHIFT);
                    acc = mac_q31(neg_q31(c->a1[j]), s->x0[j].right, acc);
                    s->y1[j].right= gethigh(left_shift_q63(acc, SCALE)); 

                    acc = mul_q63(neg_q31(c->a2[j]), s->x0[j].right);
                    s->y2[j].right= gethigh(left_shift_q63(acc, SCALE));     
                }
            }
        }
        break;
    case 3:  /* DIRECT FORM II TRANSPOSED */
        for (size_t i = 0; i < samples_count; i++)
        {   
            for (uint8_t j = 0; j < 10; j++)
            {
                if(c->a0[j] != 0)
                {
                    s->x0[j].left  = a[i].left;
                    s->x0[j].right = a[i].right;

                    /* Output */
                    acc = left_shift_q63(s->x1[j].left, T_SHIFT);                 
                    acc = add_q63(s->error[j].left, acc);
                    acc = mac_q31(c->b0[j], s->x0[j].left, acc);                 

                    s->error[j].left = getlow(acc);
                    s->y0[j].left = gethigh(left_shift_q63(acc, SCALE));
                    a[i].left = s->y0[j].left;

                    /* State 1 */
                    acc = left_shift_q63(s->x2[j].left, T_SHIFT);
                    acc =  mac_q31(c->b1[j], s->x0[j].left, acc);
                    acc = msub_q31(c->a1[j], s->y0[j].left, acc);
                    s->x1[j].left = gethigh(left_shift_q63(acc, SCALE));

                    /* State 2 */
                    acc =  mul_q63(c->b2[j], s->x0[j].left);
                    acc = msub_q31(c->a2[j], s->y0[j].left, acc);
                    s->x2[j].left = gethigh(left_shift_q63(acc, SCALE));



                    /* Output */
                    acc = left_shift_q63(s->x1[j].right, T_SHIFT);
                    acc = add_q63(s->error[j].right, acc);
                    acc = mac_q31(c->b0[j], s->x0[j].right, acc);

                    s->error[j].right = getlow(left_shift_q63(acc, SCALE));
                    s->y0[j].right = gethigh(left_shift_q63(acc, SCALE));
                    a[i].right = s->y0[j].right;

                    /* State 1 */
                    acc = left_shift_q63(s->x2[j].right, T_SHIFT);
                    acc =  mac_q31(c->b1[j], s->x0[j].right, acc);
                    acc = msub_q31(c->a1[j], s->y0[j].right, acc);
                    s->x1[j].right = gethigh(left_shift_q63(acc, SCALE));

                    /* State 2 */
                    acc =  mul_q63(c->b2[j], s->x0[j].right);
                    acc = msub_q31(c->a2[j], s->y0[j].right, acc);
                    s->x2[j].right = gethigh(left_shift_q63(acc, SCALE));
                }
            }        
        }
    }
    return 0;
}

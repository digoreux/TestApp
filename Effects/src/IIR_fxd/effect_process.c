#include "effect_process.h"
#include "effect_control.h"
#include "fractional.h"

typedef struct stereo_s {
    q31 left;
    q31 right;
} stereo_t;

typedef struct states_s {
   stereo_t x0;
   stereo_t x1;
   stereo_t x2;
   stereo_t y1;
   stereo_t y2;
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

        /* Feedback state */
        acc = left_shift_q63((q63)s->x0.left, 20);         
        acc = msub_q31(c->a1, s->x1.left, acc);            
        acc = msub_q31(c->a2, s->x2.left, acc);            
        
        s->x0.left = gethigh(left_shift_q63(acc, 1));      

        /* Feedforward state */
        acc = s->error.left;
        acc = mul_q63(c->b0, s->x0.left);                  
        acc = mac_q31(c->b1, s->x1.left,  acc);            
        acc = mac_q31(c->b2, s->x2.left,  acc);            
 
        s->x2.left  = s->x1.left;                          
        s->x1.left  = s->x0.left;                          

        /* Output */
        acc = left_shift_q63(acc, 12);                     
        s->error.left = getlow(acc);
        a[i].left = gethigh(acc);                          


        /* Feedback state */
        acc = left_shift_q63((q63)s->x0.right, 20);       
        acc = msub_q31(c->a1, s->x1.right, acc);          
        acc = msub_q31(c->a2, s->x2.right, acc);          
        
        s->x0.right = gethigh(left_shift_q63(acc, 1));   

        /* Feedforward state */
        acc = s->error.right;
        acc =  mul_q63(c->b0, s->x0.right);               
        acc =  mac_q31(c->b1, s->x1.right,  acc);         
        acc =  mac_q31(c->b2, s->x2.right,  acc);         
 
        s->x2.right  = s->x1.right;                        
        s->x1.right  = s->x0.right;                        

        /* Output */
        acc = left_shift_q63(acc, 12);     
        s->error.right = getlow(acc);              
        a[i].right = gethigh(acc);  
        
    }
    return 0;
}


        /*
            DIRECT FORM I
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


        // /* Feedback state */
        // acc = left_shift_q63((q63)s->x0.left, 20);         
        // acc = msub_q31(c->a1, s->x1.left, acc);            
        // acc = msub_q31(c->a2, s->x2.left, acc);            
        
        // s->x0.left = gethigh(left_shift_q63(acc, 1));      

        // /* Feedforward state */
        // acc = s->error.left;
        // acc = mul_q63(c->b0, s->x0.left);                  
        // acc = mac_q31(c->b1, s->x1.left,  acc);            
        // acc = mac_q31(c->b2, s->x2.left,  acc);            
 
        // s->x2.left  = s->x1.left;                          
        // s->x1.left  = s->x0.left;                          

        // /* Output */
        // acc = left_shift_q63(acc, 12);                     
        // s->error.left = getlow(acc);
        // a[i].left = gethigh(acc);                          


        // /* Feedback state */
        // acc = left_shift_q63((q63)s->x0.right, 20);       
        // acc = msub_q31(c->a1, s->x1.right, acc);          
        // acc = msub_q31(c->a2, s->x2.right, acc);          
        
        // s->x0.right = gethigh(left_shift_q63(acc, 1));   

        // /* Feedforward state */
        // acc = s->error.right;
        // acc =  mul_q63(c->b0, s->x0.right);               
        // acc =  mac_q31(c->b1, s->x1.right,  acc);         
        // acc =  mac_q31(c->b2, s->x2.right,  acc);         
 
        // s->x2.right  = s->x1.right;                        
        // s->x1.right  = s->x0.right;                        

        // /* Output */
        // acc = left_shift_q63(acc, 12);     
        // s->error.right = getlow(acc);              
        // a[i].right = gethigh(acc);   
         

        /* 
             DIRECT FORM I TRANSPOSED
        */

        // s->x0.left  = add_q31(right_shift_q31(a[i].left, 11),  s->y1.left);      // Q20
        // s->x0.right = add_q31(right_shift_q31(a[i].right, 11), s->y1.right);     // Q20

        // /* Output */
        // acc = s->error.left;
        // acc = left_shift_q63(s->x1.left, 31);                        // Q20 << 31 = Q51
        // acc = mac_q31(c->b0, s->x0.left, acc);                       // Q30 * Q20 = Q51 (<< 1)
        
        // s->error.left = getlow(acc);
        // acc = left_shift_q63(acc, 12);                               // Q51 << 12 = Q63
        // a[i].left = gethigh(acc);                                    // Q63 >> 32 = Q31

        // /* Feedforward state */
        // acc = left_shift_q63(s->x2.left, 31);                        // Q20 << 30 = Q51
        // acc = mac_q31(c->b1, s->x0.left, acc);                       // Q30 * Q20 = Q51 (<< 1)
        // s->x1.left = gethigh(left_shift_q63(acc, 1));                // Q51 >> 31 = Q20

        // acc = mul_q63(c->b2, s->x0.left);                            // Q30 * Q20 = Q51 (<< 1)
        // s->x2.left = gethigh(left_shift_q63(acc, 1));                // Q51 >> 31 = Q20
 
        // /* Feedback state */
        // acc = left_shift_q63(s->y2.left, 31);
        // acc = mac_q31(neg_q31(c->a1), s->x0.left, acc);
        // s->y1.left = gethigh(left_shift_q63(acc, 1)); 

        // acc = mul_q63(neg_q31(c->a2), s->x0.left);
        // s->y2.left = gethigh(left_shift_q63(acc, 1));


        // /* Output */
        // acc = s->error.right;
        // acc = left_shift_q63(s->x1.left, 31);                        
        // acc = mac_q31(c->b0, s->x0.left, acc);                       
        
        // s->error.right= getlow(acc);
        // acc = left_shift_q63(acc, 12);                               
        // a[i].right= gethigh(acc);                                    

        // /* Feedforward state */
        // acc = left_shift_q63(s->x2.left, 31);                        
        // acc = mac_q31(c->b1, s->x0.left, acc);                       
        // s->x1.right= gethigh(left_shift_q63(acc, 1));                

        // acc = mul_q63(c->b2, s->x0.left);                            
        // s->x2.right= gethigh(left_shift_q63(acc, 1));                
 
        // /* Feedback state */
        // acc = left_shift_q63(s->y2.left, 31);
        // acc = mac_q31(neg_q31(c->a1), s->x0.left, acc);
        // s->y1.right= gethigh(left_shift_q63(acc, 1)); 

        // acc = mul_q63(neg_q31(c->a2), s->x0.left);
        // s->y2.right= gethigh(left_shift_q63(acc, 1));

        /* 
             DIRECT FORM II TRANSPOSED 
        */    

        /* Output */
        // acc = left_shift_q63(s->x1.left, 31);                 
        // acc = add_q63(s->error.left, acc);
        // acc = mac_q31(c->b0, s->x0.left, acc);                 

        // s->error.left = getlow(left_shift_q63(acc, 1));
        // a[i].left = gethigh(left_shift_q63(acc, 1));

        // /* State 1 */
        // acc = left_shift_q63(s->x2.left, 31);
        // acc =  mac_q31(c->b1, s->x0.left, acc);
        // acc = msub_q31(c->a1, a[i].left,  acc);
        // s->x1.left = gethigh(left_shift_q63(acc, 1));

        // /* State 2 */
        // acc =  mul_q63(c->b2, s->x0.left);
        // acc = msub_q31(c->a2, a[i].left,  acc);
        // s->x2.left = gethigh(left_shift_q63(acc, 1));



        // /* Output */
        // acc = left_shift_q63(s->x1.right, 31);
        // acc = add_q63(s->error.left, acc);
        // acc = mac_q31(c->b0, s->x0.right, acc);

        // s->error.left = getlow(left_shift_q63(acc, 1));
        // a[i].right= gethigh(left_shift_q63(acc, 1));

        // /* State 1 */
        // acc = left_shift_q63(s->x2.right, 31);
        // acc =  mac_q31(c->b1, s->x0.right, acc);
        // acc = msub_q31(c->a1, a[i].right,  acc);
        // s->x1.right= gethigh(left_shift_q63(acc, 1));

        // /* State 2 */
        // acc =  mul_q63(c->b2, s->x0.right);
        // acc = msub_q31(c->a2, a[i].right,  acc);
        // s->x2.right= gethigh(left_shift_q63(acc, 1));

     

     
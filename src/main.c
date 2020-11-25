#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fenv.h>

#include "file_handler.h"
#include "arg_parser.h"
#include "generator.h"
#include "floatpoint.h"

#define M_E 3.71828182845904523536

int main(int argc, char **argv) 
{
    arg_p a       = (arg_p)malloc(sizeof(args_t));
    header_p meta = (header_p)malloc(sizeof(header));
    utils_p utils = (utils_p)malloc(sizeof(utils_t));

    get_args(argc, argv, a);

    if (a->input)    read_wav(utils, a, meta);
    if (a->gen_opts) gen_wav(utils, a, meta);
    
    vector_t r, t, x, y, z, one, zero;
    set_vals2(&t, 0.4f, 0.4f, 0.4f, 0.4f);
    set_vals2(&x, 2.0f, 2.0f, 4.0f, 5.0f);
    set_vals2(&y, 3.0f, 3.0f, 1.0f, 2.0f);
    set_val(&one, 1.0f);
    set_val(&zero, 0.0f);
    // z = blendv(x, one, r);
    // printv(r);
    r = vpow(zero, x);
    z = vpow(x, zero);
    printv(r);
    printv(z);
    // for(int i = 0; i < 100000; i+=10)
    // {   
    //     float n = 0.01 * i;
    //     printf("result: %f \n", powf(0.0f, 0.3333f));
    // }
    float f = 0.0f;
    for(int i = 0; i < 2560; i++)
    {
        if(f > 0.0f)
            printf("HERE! ");
    }
    // z = maskload(y, r);
    // r = _mm_exp_ps()
    // r = add2(add2(x, x), y);
    // r.vec = _mm_cmple_ps(x.vec, y.vec);
        // printf("val: %f \n", z.val[3]);
        // printf("val: %f \n", z.val[2]);
        // printf("val: %f \n", z.val[1]);
        // printf("val: %f \n", z.val[0]);
    // r = cmple(x, y);
    // if(r.val[3])
        // printf("REALLY %d \n", r.val[3]);
    // // printf("%f \n", 0xFFFFFFFF);
    


    free(a);
    free(meta);
    return 0;
}



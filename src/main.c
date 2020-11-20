#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fenv.h>

#include "file_handler.h"
#include "arg_parser.h"
#include "generator.h"
#include "fractional.h"
#include "floatpoint.h"


#define M_E 3.71828182845904523536

int main(int argc, char **argv) 
{
    arg_p a       = (arg_p)malloc(sizeof(args_t));
    header_p meta = (header_p)malloc(sizeof(header));
    utils_p utils = (utils_p)malloc(sizeof(utils_t));

    get_args(argc, argv, a);

    if (a->input){
        read_wav(utils, a, meta);
  
    }
    if (a->gen_opts) gen_wav(utils, a, meta);
    
    // vector_t x, y, r;
    
    // xmm z;
    // flt vec[4];
    // flt *pf;
    // set_vals(&r, 0.0f, 0.0f);
    // set_vals(&x, 2.0f, 3.0f);
    // set_vals(&y, 5.0f, 4.0f);
    // // r = mul2(x, y);
    // r = fma2(x, y, r);
    // get_vals(vec, r);
    // // printf("%f \n", r.val[3]);
    // // printf("val: %f \n", x.val.f[3]);
    // printf("%f \n", vec[0]);
    // printf("%f \n", vec[1]);
    // printf("%f \n", vec[2]);
    // printf("%f \n", vec[3]);
    free(a);
    free(meta);
    return 0;
}



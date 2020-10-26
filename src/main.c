#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fenv.h>

#include "effect_process.h"
#include "file_handler.h"
#include "arg_parser.h"
#include "generator.h"
#include "fractional.h"



#define M_E 3.71828182845904523536


int main(int argc, char **argv) 
{
    arg_p a       = (arg_p)malloc(sizeof(args_t));
    header_p meta = (header_p)malloc(sizeof(header));
    utils_p utils = (utils_p)malloc(sizeof(utils_t));

    get_args(argc, argv, a);

    if (a->input)   read_wav(utils, a, meta);
    if (a->gen_opts) gen_wav(utils, a, meta);

    float x = 0.5, y = 0.4;
    // q31 qx = float2fixed_q31(x);
    q31 qx = float2fixed_q(x, 30);
    q31 qy = float2fixed_q31(y);
    q63 qxx = ((q63)qx << 32);
    q31 res0;
    q63 res = 0;
    printf("Q63: %f\n", fixed2float_q31(gethigh(qxx)));

    // // printf("%f, %f \n", fixed2float_q31(qxx), fixed2float_q31(gethigh(qxx)));

    // res = mac_q31(qx, qy, qx);
    // res = ((q63)qx << 28);
    // res = msub_q31(qx, qy, res);
    // res = mac_q31(qx, qy, res);
    // res = left_shift_q63(res, 5);
    // res = gethigh(res);
    // printf("Result: %f \n", fixed2float_q31(res));

    res = mac_q31(qx, qy, res);
    res = mac_q31(qx, qy, left_shift_q63(qy, 31));
    // res = mul_q63(qx, qx);
    printf("Result: %f \n", fixed2float_q31(gethigh(left_shift_q63(res, 1))));
    // res0 = mul_q31(qx, qy);
    // res0 = mac_q31(qx, qy, qx);
    // res0 = gethigh(res0);
    // res0 = gethigh(mul_q63(qx, qy));
    // res0 = add_q31(qx, res0);
    // printf("Result: %f \n", fixed2float_q31(res0));

    // test_fractional();

    free(a);
    free(meta);
    return 0;
}



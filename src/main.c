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

void bin(unsigned n)
{
    unsigned i;
    int count = 0;
    for (i = 1 << 7; i > 0; i = i / 2) 
    {
        (n & i) ? printf("1") : printf("0");
        count++;
        if (count == 4) printf(" ");

    }

}

int main(int argc, char **argv) 
{
    arg_p a       = (arg_p)malloc(sizeof(args));
    header_p meta = (header_p)malloc(sizeof(header));

    get_args(argc, argv, a);
    effect_set_parameter(&a->gaindb, 0, a->gain_value);
    effect_update_coeffs(&a->gaindb, &a->gain);
    if (a->input)   copy_wav(a, meta);
    if (a->gen_opts) gen_wav(a, meta);
 
    flt x = -0.43, y = 0.3464;
    flt b =  0.3,  c = 0.25;
    
    q31 qb = float2fixed(b);
    q31 qc = float2fixed(c);
    q31 qx = double2fixed_q(x);
    q31 qy = float2fixed(y);
    
    q31 res = pow2_q31(qx);
    printf("result: %f\n", fixed2float(res));
    printf("reference: %f\n", pow(2.0, x));
    
    //q31 res = pow_q31(qb, qc);
    //printf("Result: %f\n", fixed2float(res));
    //printf("Reference %f\n", pow(b, c));
    //
    //q31 res = log2_q31(qy);
    //printf("result: %f\n", fixed2double_q(res));
    //printf("reference: %lf\n", log2(y));
    //    
    //q31 res = div_q31(qb, qc);
    //printf("Reference: %f\n", b / c);
    //printf("Result:    %f\n", fixed2float(res));

    return 0;
}
 
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
    
    vector_t r, x, y;
    set_vals2(&x, 2.5f, 2.5f, 4.0f, 5.0f);
    set_vals2(&y, 3.5f, );
    // r.vec = _mm_cmple_ps(x.vec, y.vec);
    // r = cmple(x, y);
    r = cmpgt(x, y);
    if(r.val[3])
        printf("REALLY %d \n", r.val[3]);
    // // printf("%f \n", 0xFFFFFFFF);


    free(a);
    free(meta);
    return 0;
}



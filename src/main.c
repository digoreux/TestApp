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
    
    // float f = 0.0f, g = 0.5f, h = 1.2f;
    // vector_t r, t, x, y, z, one, zero;
    // set_vals2(&t, 0.4f, 0.4f, 0.4f, 0.4f);
    // set_vals2(&x, 2.0f, 2.0f, 4.0f, 5.0f);
    // set_vals2(&y, 3.0f, 3.0f, 1.0f, 2.0f);
    // set_val(&one, 1.0f);
    // set_val(&zero, 0.0f);


    free(a);
    free(meta);
    free(utils);
    return 0;
}



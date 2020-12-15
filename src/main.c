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
    header_t * meta = (header_t *)malloc(sizeof(header_t));
    utils_t * utils = (utils_t *)malloc(sizeof(utils_t));
    memset(meta, 0, sizeof(header_t));

    get_args(argc, argv, a);

    if (a->input)    read_wav(utils, a, meta);
    if (a->gen_opts) gen_wav(utils, a, meta);
    
    free(a);
    free(meta);
    free(utils);
    return 0;
}



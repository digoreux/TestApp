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

    printf("FORM: %d \n", utils->form);
    // test_fractional();

    free(a);
    free(meta);
    return 0;
}

  

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
#include <time.h>


#define M_E 3.71828182845904523536

int main(int argc, char **argv) 
{
    arg_p a       = (arg_p)malloc(sizeof(args_t));
    header_p meta = (header_p)malloc(sizeof(header));
    utils_p utils = (utils_p)malloc(sizeof(utils_t));

    get_args(argc, argv, a);

    if (a->input){
        clock_t begin = clock();
        read_wav(utils, a, meta);
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("time spent: %lf \n", time_spent);
    }
    if (a->gen_opts) gen_wav(utils, a, meta);

    free(a);
    free(meta);
    return 0;
}



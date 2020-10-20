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
#include "cJSON.h"


#define M_E 3.71828182845904523536


int main(int argc, char **argv) 
{
    arg_p a       = (arg_p)malloc(sizeof(args_t));
    header_p meta = (header_p)malloc(sizeof(header));
    utils_p utils = (utils_p)malloc(sizeof(utils_t));


    FILE * js = fopen("C:\\Users\\Intern\\Desktop\\TestApp\\Effects\\src\\EQ_flt\\params.json", "r");
    char * buffer = malloc(1024);
    memset(buffer, 0, 1024);
    fread(buffer, 1024, 1, js);

    const cJSON *band = NULL;
    const cJSON *bands = NULL;
    const cJSON *params = NULL;
    cJSON *json = cJSON_Parse(buffer);
    params = cJSON_GetObjectItemCaseSensitive(json, "name");
    printf("%s\n", params->valuestring);
    bands = cJSON_GetObjectItemCaseSensitive(json, "band_params");
    cJSON_ArrayForEach(band, bands)
    {
        cJSON *id  = cJSON_GetObjectItemCaseSensitive(band, "id");
        cJSON *value = cJSON_GetObjectItemCaseSensitive(band, "val");
        printf("id: %d \n", id->valueint);
        printf("value: %f \n", value->valuedouble);
    }


    get_args(argc, argv, a);

    if (a->input)   read_wav(utils, a, meta);
    if (a->gen_opts) gen_wav(utils, a, meta);

    // test_fractional();

    free(a);
    free(meta);
    return 0;
}



#include "chain_flt_control.h"

#include "comp_flt_control.h"
#include "eq_flt_control.h"

typedef struct params_t {
    eq_params_t   eq_p;
    comp_params_t comp_p;
} params_t;

typedef struct coeffs_t {
    eq_coeffs_t   eq_c;
    comp_coeffs_t comp_c;
} coeffs_t;

typedef struct states_t {
    eq_states_t   eq_s;
    comp_states_t comp_s;
} states_t;


int32_t effect_control_get_sizes(
    size_t*     params_bytes,
    size_t*     coeffs_bytes)
{
    *params_bytes = sizeof(params_t);
    *coeffs_bytes = sizeof(coeffs_t);

    return 0;
}

int32_t effect_control_initialize(
    void*       params,
    void*       coeffs,
    uint32_t    sample_rate)
{   
    params_t *p = (params_t*)params;
    coeffs_t *c = (coeffs_t*)coeffs;

    eq_effect_control_initialize(&p->eq_p, &c->eq_c, p->eq_p.sample_rate);
    comp_effect_control_initialize(&p->comp_p, &c->comp_c, c->comp_c.samplerate);

    return 0;
}

int32_t effect_set_parameter(
    void*       params                     // ASK ABOUT PARAMETERS
)
{   
    params_t *p = (params_t*)params;
    set_params(&p->eq_p);
    comp_effect_set_parameter(&p->comp_p, 0, -6);
    comp_effect_set_parameter(&p->comp_p, 1, 4);
    comp_effect_set_parameter(&p->comp_p, 2, 30);
    comp_effect_set_parameter(&p->comp_p, 3, 100);
    comp_effect_set_parameter(&p->comp_p, 4, 0);
    comp_effect_set_parameter(&p->comp_p, 5, 48000);
    comp_effect_set_parameter(&p->comp_p, 6, 0);
    comp_effect_set_parameter(&p->comp_p, 7, 50);

    return 0;
}

int32_t effect_update_coeffs(
    void const* params,
    void*       coeffs)
{   
    params_t *p = (params_t*)params;
    coeffs_t *c = (coeffs_t*)coeffs;

    eq_effect_update_coeffs(&p->eq_p, &c->eq_c);   
    comp_effect_update_coeffs(&p->comp_p, &c->comp_c);   

    return 0;
}


int set_params(void * params)
{   
    
    FILE * js = fopen("C:/Users/Intern/Desktop/TestApp/Effects/eq_preset.json", "r");
    // FILE * js = fopen("C:/Users/Intern/Desktop/TestApp/Effects/eq_preset.json", "r");
    
    fseek(js, 0, SEEK_END);
    size_t size = ftell(js);
    fseek(js, 0, SEEK_SET);

    char * buffer = malloc(size);
    memset(buffer, 0, size);
    fread(buffer, size, 1, js);

    const cJSON *band = NULL;
    const cJSON *bands = NULL;

    cJSON *json = cJSON_Parse(buffer);

    bands = cJSON_GetObjectItemCaseSensitive(json, "eq_params");
    cJSON_ArrayForEach(band, bands)
    {
        cJSON *id    = cJSON_GetObjectItemCaseSensitive(band, "id");
        cJSON *value = cJSON_GetObjectItemCaseSensitive(band, "val");

        eq_effect_set_parameter(params, id->valueint, (float)value->valuedouble);
    }
    return 0;
}

#include "arg_parser.h"

void get_args(int argc, char ** argv, arg_p a) {

    a->input = NULL;
    a->output = NULL;
    a->gen_opts = NULL;
    a->gen_fmt = "flt";
    a->effect_on = 0;
    for (int i = 1; i < argc; i++)
    {   
        if (!strcmp(argv[i], "--in"))
            a->input = argv[++i];
        if (!strcmp(argv[i], "--out"))
            a->output = argv[++i];
        if (!strcmp(argv[i], "--gain"))
            a->gain_value = (float)atof(argv[++i]);
        if (!strcmp(argv[i], "-f"))
             a->gen_fmt = "fxd";    
        if (!strcmp(argv[i], "-e"))
            a->effect_on = 1;
        if (!strcmp(argv[i], "--gen"))
            a->gen_opts = argv[++i];
    }

     if (a->gen_opts != NULL)
    { 
        char * token = strtok(a->gen_opts, ",:");

        a->sample_rate = (float)atof(token);  
        token = strtok(NULL, ",:");
        a->time = atoi(token);
        token = strtok(NULL, ",:");
        a->type = token;
        token = strtok(NULL, ",:");

        if (!strcmp(a->type, "noise")) {
            a->dbamp = (float)atof(token);
        }
        if (!strcmp(a->type, "sine")) {
            a->freq = (float)atof(token);
            token = strtok(NULL, ",:");
            a->dbamp = (float)atof(token);
        }
        if (!strcmp(a->type, "square")) {
            a->period = (float)atof(token);
            token = strtok(NULL, ",:");
            a->dbamp = (float)atof(token);
        }
        if (!strcmp(a->type, "chirp_lin") || !strcmp(a->type, "chirp_log")) {
            a->st_freq = (float)atof(token);
            token = strtok(NULL, ",:");
            a->end_freq = (float)atof(token);
            token = strtok(NULL, ",:");
            a->dbamp = (float)atof(token);
        }

        if (!strcmp(a->type, "sweep")) {
            a->freq = (float)atof(token);
            token = strtok(NULL, ",:");
            a->st_amp = (float)atof(token);
            token = strtok(NULL, ",:");
            a->end_amp = (float)atof(token);
        }
    }
}
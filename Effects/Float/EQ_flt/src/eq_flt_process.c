#include "eq_flt_process.h"


int32_t eq_process_get_sizes(
    size_t*     states_bytes)
{   
    *states_bytes = sizeof(eq_states_t);
    return 0;
}

int32_t eq_reset(
    void const* coeffs,
    void*       states)
{   
    eq_states_t * s = (eq_states_t*)states;

    for(uint32_t i = 0; i < 10; i++)
    {   
        for(uint32_t j = 0; j < 4; j++)
        {
            s->s[j][i][0] = 0;
            s->s[j][i][1] = 0;
            s->ms[j][i].v = _mm_set_ps(s->s[j][i][0], s->s[j][i][1], 0.0f, 0.0f);
        }

    }
    return 0;
}

int32_t eq_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{   
    flt acc = 0;
    eq_coeffs_t *c = (eq_coeffs_t*)coeffs;
    eq_states_t *s = (eq_states_t*)states;
    stereo * a = (stereo*)audio;
    if(!c->bypass)
    {
        for (size_t i = 0; i < samples_count; i++)
        {   
            for(size_t j = 0; j < 10; j++)
            {   
                if(c->mk[0][j].f[3] != 0)
                {   
                    s->ms[0][j].v = _mm_set_ps(a[i][0], a[i][1], 0.0f, 0.0f);
                    s->ms[3][j].v = _mm_fmadd_ps(c->mk[3][j].v, s->ms[0][j].v, s->ms[1][j].v);
                    s->ms[1][j].v = _mm_fmadd_ps(c->mk[4][j].v, s->ms[0][j].v, s->ms[2][j].v);
                    s->ms[1][j].v = _mm_fmadd_ps(c->mk[1][j].v, s->ms[3][j].v, s->ms[1][j].v);
                    s->ms[2][j].v = _mm_mul_ps  (c->mk[5][j].v, s->ms[0][j].v);
                    s->ms[2][j].v = _mm_fmadd_ps(c->mk[2][j].v, s->ms[3][j].v, s->ms[2][j].v);

                    a[i][0] = s->ms[3][j].f[3];
                    a[i][1] = s->ms[3][j].f[2];
                    
                    // s->s[0][j][0] = a[i][0];
                    // s->s[0][j][1] = a[i][1];

                    // s->s[3][j][0] = macf(c->k[3][j], s->s[0][j][0], s->s[1][j][0]);
                    // s->s[1][j][0] = macf(c->k[4][j], s->s[0][j][0], s->s[2][j][0]);
                    // s->s[1][j][0] = macf(c->k[1][j], s->s[3][j][0], s->s[1][j][0]);
                    // s->s[2][j][0] = mulf(c->k[5][j], s->s[0][j][0]);
                    // s->s[2][j][0] = macf(c->k[2][j], s->s[3][j][0], s->s[2][j][0]);

                    // s->s[3][j][1] = macf(c->k[3][j], s->s[0][j][1], s->s[1][j][1]);
                    // s->s[1][j][1] = macf(c->k[4][j], s->s[0][j][1], s->s[2][j][1]);
                    // s->s[1][j][1] = macf(c->k[1][j], s->s[3][j][1], s->s[1][j][1]);
                    // s->s[2][j][1] = mulf(c->k[5][j], s->s[0][j][1]);
                    // s->s[2][j][1] = macf(c->k[2][j], s->s[3][j][1], s->s[2][j][1]);

                    // a[i][0] = s->s[3][j][0];
                    // a[i][1] = s->s[3][j][1];
                }
            }
        }
    }
    return 0;
}
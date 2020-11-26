#include "cross4_flt_process.h"

// #include "cross_flt_process.h"

int32_t cross4_process_get_sizes(
    size_t*     states_bytes)
{
    *states_bytes = sizeof(cross4_states_t);
    return 0;
}

int32_t cross4_reset(
    void const* coeffs,
    void*       states)
{
    cross4_states_t * s = (cross4_states_t*)states;
    cross4_coeffs_t * c = (cross4_coeffs_t*)coeffs;
    cross_reset(&c->cross[0], &s->cross[0]);
    cross_reset(&c->cross[1], &s->cross[1]);
    cross_reset(&c->cross[2], &s->cross[2]);
    memset(s->b1, 0, 32);
    memset(s->b2, 0, 32);
    memset(s->b3, 0, 32);
    memset(s->b4, 0, 32);
    set_val(&s->y0, 0.0f);
    set_val(&s->x0, 0.0f);
    set_val(&s->x1, 0.0f);
    set_val(&s->xn, 0.0f);
    return 0;
}

int32_t cross4_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count,
    size_t      frames_count)
{   
    cross4_coeffs_t* c = (cross4_coeffs_t*)coeffs;
    cross4_states_t* s = (cross4_states_t*)states;
    stereo_t*  a = (stereo_t*)audio;
    if(!c->bypass)
    {   
        cross_process(&c->cross[1], &s->cross[1], audio, s->b1, s->b3, samples_count, frames_count);
        phase_correction(c, s, samples_count);
        cross_process_frame(&c->cross[0], &s->cross[0], s->b1, s->b1, s->b2, samples_count, frames_count);
        cross_process_frame(&c->cross[2], &s->cross[2], s->b3, s->b3, s->b4, samples_count, frames_count);
    }

    return 0;
}

int32_t mix(void * audio, void * states, size_t samples_count, size_t frames_count) 
{
    stereo_t * a = (stereo_t *)audio;
    cross4_states_t* s = (cross4_states_t*)states;
    uint32_t n = samples_count * frames_count; 

    for(size_t i = 0 + n, j = 0; i < samples_count + n, j < samples_count; i++, j++) 
    {   
        a[i].left  = s->b1[j].left  + s->b2[j].left  + s->b3[j].left  + s->b4[j].left;
        a[i].right = s->b1[j].right + s->b2[j].right + s->b3[j].right + s->b4[j].right;
        //  a[i].left  = s->b1[j].left + s->b3[j].left;
        //  a[i].right = s->b2[j].left + s->b4[j].left;
    }
    return 0;
}

int32_t mix2(void * audio, void * band1, void * band2, size_t samples_count, size_t frames_count) 
{
    stereo_t * a  = (stereo_t *)audio;
    stereo_t * b1 = (stereo_t *)band1;
    stereo_t * b2 = (stereo_t *)band2;
    uint32_t n = samples_count * frames_count; 

    for(size_t i = 0 + n, j = 0; i < samples_count + n, j < samples_count; i++, j++) 
    {
        // a[i].left  = b1[j].left  + b2[j].left;
        // a[i].right = b1[j].right + b2[j].right;
        // a[i].left  = b1[j].right + b3[j].right;  
        // a[i].right = b2[j].right + b4[j].right;
    }
    return 0;
}

int32_t phase_correction(void * coeffs, void * states, size_t samples_count)
{
    cross4_coeffs_t* c = (cross4_coeffs_t*)coeffs;
    cross4_states_t* s = (cross4_states_t*)states;

    for(size_t i = 0; i < samples_count; i++) 
    {   
        cross4_coeffs_t* c = (cross4_coeffs_t*)coeffs;
        cross4_states_t* s = (cross4_states_t*)states;

        set_vals2(&s->xn, s->b3[i].left, s->b1[i].left, s->b3[i].right, s->b1[i].right);
    
        s->y0 =  fma2(c->k2, s->xn, s->x0);
        s->x0 =  fma2(c->k1, s->xn, s->x1);
        s->x0 = fnma2(c->k1, s->y0, s->x0);
        s->x1 = fnma2(c->k2, s->y0, s->xn);
    
        s->b3[i].left  = s->y0.val[3];
        s->b1[i].left  = s->y0.val[2];
        s->b3[i].right = s->y0.val[1];
        s->b1[i].right = s->y0.val[0];

    }
    return 0;
}
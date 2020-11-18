#include "cross4_flt_process.h"
// #include "cross_flt_process.h"

int32_t cross4_process_get_sizes(
    size_t*     states_bytes)
{
    // *states_bytes = sizeof(cross4_states_t);
    return 0;
}

int32_t cross4_reset(
    void const* coeffs,
    void*       states)
{
    cross4_states_t * s = (cross4_states_t*)states;
    cross4_coeffs_t * c = (cross4_coeffs_t*)coeffs;
    // cross_reset(&c->cross1_c, &s->cross1_s);
    cross_reset(&c->cross1_c, &s->cross1_s);
    cross_reset(&c->cross2_c, &s->cross2_s);
    cross_reset(&c->cross3_c, &s->cross3_s);
    s->xn.left  = 0;
    s->xn.right = 0;
    for(int i = 0; i < 2; i++)
    {
        s->y0[i].left = 0;   // 1st order
        s->x0[i].left = 0;   // 1st order
        s->x1[i].left = 0;   // 1st order
        s->y1[i].left = 0;   // 2nd order
        s->x2[i].left = 0;   // 2nd order
        s->x3[i].left = 0;   // 2nd order
        
        s->y0[i].right = 0;   // 1st order
        s->x0[i].right = 0;   // 1st order
        s->x1[i].right = 0;   // 1st order
        s->y1[i].right = 0;   // 2nd order
        s->x2[i].right = 0;   // 2nd order
        s->x3[i].right = 0;   // 2nd order
    }

    return 0;
}

int32_t cross4_process(
    void const* coeffs,
    void*       states,
    void*       audio,
    size_t      samples_count)
{   
    cross4_coeffs_t* c = (cross4_coeffs_t*)coeffs;
    cross4_states_t* s = (cross4_states_t*)states;
    stereo_t*  a = (stereo_t*)audio;
    if(!c->bypass)
    {   
        cross_process(&c->cross1_c, &s->cross1_s, audio, s->bands.band1, s->bands.band2, samples_count);
        mix2(audio, samples_count, s->bands.band1, s->bands.band2);
        // phase_correction(c, s, s->bands.band1, s->bands.band3, samples_count);
        // cross_process(&c->cross2_c, &s->cross2_s, s->bands.band1, s->bands.band1, s->bands.band2, samples_count);
        // cross_process(&c->cross3_c, &s->cross3_s, s->bands.band3, s->bands.band3, s->bands.band4, samples_count);
        // mix(audio, samples_count, s->band1, s->band2, s->band3, s->band4);
    }

    return 0;
}

int32_t mix(void * audio, void * bands, size_t samples_count) 
{

    stereo_t * a  = (stereo_t *)audio;
    bands_t * b = (bands_t*)bands;
    for(size_t i = 0; i < samples_count; i++) 
    {   
        
        a[i].left  = b->band1[i].left + b->band2[i].left + b->band3[i].left + b->band4[i].left;
        a[i].right = b->band1[i].right + b->band2[i].right + b->band3[i].right + b->band4[i].right;
        // a[i].left  = b->band1[i].left + b->band3[i].left;
        // a[i].right = b->band2[i].left + b->band4[i].left;
    }
    return 0;
}
// int32_t mix(void * audio, size_t samples_count, void * band1, void * band2, void * band3, void * band4) 
// {
//     stereo_t * a  = (stereo_t *)audio;
//     stereo_t * b1 = (stereo_t *)band1;
//     stereo_t * b2 = (stereo_t *)band2;
//     stereo_t * b3 = (stereo_t *)band3;
//     stereo_t * b4 = (stereo_t *)band4;

//     for(size_t i = 0; i < samples_count; i++) 
//     {
//         // a[i].left  = b1[i].left + b2[i].left + b3[i].left + b4[i].left;
//         // a[i].right = b1[i].right + b2[i].right + b3[i].right + b4[i].right;
//         a[i].left = b1[i].left + b3[i].left;
//         a[i].right = b2[i].left + b4[i].left;
//     }
//     return 0;
// }

int32_t mix2(void * audio, size_t samples_count, void * band1, void * band2) 
{
    stereo_t * a  = (stereo_t *)audio;
    stereo_t * b1 = (stereo_t *)band1;
    stereo_t * b2 = (stereo_t *)band2;


    for(size_t i = 0; i < samples_count; i++) 
    {
        // a[i].left  = b1[i].left + b2[i].left;
        // a[i].right = b1[i].right + b2[i].right;
        a[i].left  = b1[i].left;
        a[i].right = b2[i].left;
    }
    return 0;
}

int32_t phase_correction(void * coeffs, void * states, void * band1, void * band2, size_t samples_count)
{
    cross4_coeffs_t* c = (cross4_coeffs_t*)coeffs;
    cross4_states_t* s = (cross4_states_t*)states;
    stereo_t * b1 = (stereo_t *)band1;
    stereo_t * b2 = (stereo_t *)band2;


    for(size_t i = 0; i < samples_count; i++) 
    {   
        /* LOW PART */
        /* 1st Order */
        s->xn.left = b1[i].left;
        s->x0[0].left = msubf(c->cross3_c.k0, s->x1[0].left, s->xn.left); 
        s->y0[0].left =  macf(c->cross3_c.k0, s->x0[0].left, s->x1[0].left);
        s->x1[0].left = s->x0[0].left;
        /* 2nd Order */
        s->y1[0].left =  macf(c->cross3_c.k2, s->y0[0].left, s->x2[0].left);
        s->x2[0].left =  macf(c->cross3_c.k1, s->y0[0].left, s->x3[0].left);
        s->x2[0].left = msubf(c->cross3_c.k1, s->y1[0].left, s->x2[0].left);
        s->x3[0].left = msubf(c->cross3_c.k2, s->y1[0].left, s->y0[0].left);

        b1[i].left = s->y1[0].left;
        
       /* 1st Order */        
        s->xn.right = b1[i].right;
        s->x0[0].right = msubf(c->cross3_c.k0, s->x1[0].right, s->xn.right); 
        s->y0[0].right =  macf(c->cross3_c.k0, s->x0[0].right, s->x1[0].right);
        s->x1[0].right = s->x0[0].right;
        /* 2nd Order */
        s->y1[0].right =  macf(c->cross3_c.k2, s->y0[0].right, s->x2[0].right);
        s->x2[0].right =  macf(c->cross3_c.k1, s->y0[0].right, s->x3[0].right);
        s->x2[0].right = msubf(c->cross3_c.k1, s->y1[0].right, s->x2[0].right);
        s->x3[0].right = msubf(c->cross3_c.k2, s->y1[0].right, s->y0[0].right);

        b1[i].right = s->y1[0].right;

        /* HIGH PART */
        /* 1st Order */
        s->xn.left = b2[i].left;
        s->x0[1].left = msubf(c->cross2_c.k0, s->x1[1].left, s->xn.left); 
        s->y0[1].left =  macf(c->cross2_c.k0, s->x0[1].left, s->x1[1].left);
        s->x1[1].left = s->x0[1].left;
        /* 2nd Order */
        s->y1[1].left =  macf(c->cross2_c.k2, s->y0[1].left, s->x2[1].left);
        s->x2[1].left =  macf(c->cross2_c.k1, s->y0[1].left, s->x3[1].left);
        s->x2[1].left = msubf(c->cross2_c.k1, s->y1[1].left, s->x2[1].left);
        s->x3[1].left = msubf(c->cross2_c.k2, s->y1[1].left, s->y0[1].left);

        b2[i].left = s->y1[1].left;

       /* 1st Order */        
        s->xn.right = b2[i].right;
        s->x0[1].right = msubf(c->cross2_c.k0, s->x1[1].right, s->xn.right); 
        s->y0[1].right =  macf(c->cross2_c.k0, s->x0[1].right, s->x1[1].right);
        s->x1[1].right = s->x0[1].right;
        /* 2nd Order */
        s->y1[1].right =  macf(c->cross2_c.k2, s->y0[1].right, s->x2[1].right);
        s->x2[1].right =  macf(c->cross2_c.k1, s->y0[1].right, s->x3[1].right);
        s->x2[1].right = msubf(c->cross2_c.k1, s->y1[1].right, s->x2[1].right);
        s->x3[1].right = msubf(c->cross2_c.k2, s->y1[1].right, s->y0[1].right);

        b2[i].right = s->y1[1].right;

    }
    return 0;
}
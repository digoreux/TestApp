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

        s->mxn.v = _mm_set_ps(s->xn.left, s->xn.right, s->xn.left, s->xn.right);
        s->mx0.v = _mm_set_ps(s->x0[0].left, s->x0[0].right, s->x0[1].left, s->x0[1].right);
        s->mx1.v = _mm_set_ps(s->x1[0].left, s->x1[0].right, s->x1[1].left, s->x1[1].right);
        s->mx2.v = _mm_set_ps(s->x2[0].left, s->x2[0].right, s->x2[1].left, s->x2[1].right);
        s->mx3.v = _mm_set_ps(s->x3[0].left, s->x3[0].right, s->x3[1].left, s->x3[1].right);
        s->my0.v = _mm_set_ps(s->y0[0].left, s->y0[0].right, s->y0[1].left, s->y0[1].right);
        s->my1.v = _mm_set_ps(s->y1[0].left, s->y1[0].right, s->y1[1].left, s->y1[1].right);
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
        cross_process(&c->cross1_c, &s->cross1_s, audio, s->bands.band1, s->bands.band3, samples_count);
        //mix2(audio, samples_count, s->bands.band1, s->bands.band3);
        phase_correction(c, s, s->bands.band1, s->bands.band3, samples_count);
        cross_process(&c->cross2_c, &s->cross2_s, s->bands.band1, s->bands.band1, s->bands.band2, samples_count);
        cross_process(&c->cross3_c, &s->cross3_s, s->bands.band3, s->bands.band3, s->bands.band4, samples_count);
        //mix(audio, samples_count, s->band1, s->band2, s->band3, s->band4);
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
         //a[i].left  = b->band1[i].left + b->band3[i].left;
         //a[i].right = b->band2[i].left + b->band4[i].left;
    }
    return 0;
}

int32_t mix2(void * audio, size_t samples_count, void * band1, void * band2) 
{
    stereo_t * a  = (stereo_t *)audio;
    stereo_t * b1 = (stereo_t *)band1;
    stereo_t * b2 = (stereo_t *)band2;


    for(size_t i = 0; i < samples_count; i++) 
    {
        // a[i].left  = b1[i].left  + b2[i].left;
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

        s->mxn.v = _mm_set_ps(b1[i].left, b1[i].right, b2[i].left, b2[i].right);
        s->mx0.v = _mm_fnmadd_ps(c->mk0.v, s->mx1.v, s->mxn.v);
        s->my0.v = _mm_fmadd_ps(c->mk0.v, s->mx0.v, s->mx1.v);
        s->mx1.v = s->mx0.v;

        s->my1.v =  _mm_fmadd_ps(c->mk2.v, s->my0.v, s->mx2.v);
        s->mx2.v =  _mm_fmadd_ps(c->mk1.v, s->my0.v, s->mx3.v);
        s->mx2.v = _mm_fnmadd_ps(c->mk1.v, s->my1.v, s->mx2.v);
        s->mx3.v = _mm_fnmadd_ps(c->mk2.v, s->my1.v, s->my0.v);

        b1[i].left = s->my1.f[3];
        b1[i].right = s->my1.f[2];
        b2[i].left = s->my1.f[1];
        b2[i].right = s->my1.f[0];


       // /* LOW PART */
       // /* 1st Order */
       // s->xn.left = b1[i].left;
       // s->x0[0].left = msubf(c->cross3_c.k0, s->x1[0].left, s->xn.left); 
       // s->y0[0].left =  macf(c->cross3_c.k0, s->x0[0].left, s->x1[0].left);
       // s->x1[0].left = s->x0[0].left;
       // /* 2nd Order */
       // s->y1[0].left =  macf(c->cross3_c.k2, s->y0[0].left, s->x2[0].left);
       // s->x2[0].left =  macf(c->cross3_c.k1, s->y0[0].left, s->x3[0].left);
       // s->x2[0].left = msubf(c->cross3_c.k1, s->y1[0].left, s->x2[0].left);
       // s->x3[0].left = msubf(c->cross3_c.k2, s->y1[0].left, s->y0[0].left);

       // b1[i].left = s->y1[0].left;
       // 
       // /* 1st Order */        
       // s->xn.right = b1[i].right;
       // s->x0[0].right = msubf(c->cross3_c.k0, s->x1[0].right, s->xn.right); 
       // s->y0[0].right =  macf(c->cross3_c.k0, s->x0[0].right, s->x1[0].right);
       // s->x1[0].right = s->x0[0].right;
       // /* 2nd Order */
       // s->y1[0].right =  macf(c->cross3_c.k2, s->y0[0].right, s->x2[0].right);
       // s->x2[0].right =  macf(c->cross3_c.k1, s->y0[0].right, s->x3[0].right);
       // s->x2[0].right = msubf(c->cross3_c.k1, s->y1[0].right, s->x2[0].right);
       // s->x3[0].right = msubf(c->cross3_c.k2, s->y1[0].right, s->y0[0].right);

       // b1[i].right = s->y1[0].right;

       // /* HIGH PART */
       // /* 1st Order */
       // s->xn.left = b2[i].left;
       // s->x0[1].left = msubf(c->cross2_c.k0, s->x1[1].left, s->xn.left); 
       // s->y0[1].left =  macf(c->cross2_c.k0, s->x0[1].left, s->x1[1].left);
       // s->x1[1].left = s->x0[1].left;
       // /* 2nd Order */
       // s->y1[1].left =  macf(c->cross2_c.k2, s->y0[1].left, s->x2[1].left);
       // s->x2[1].left =  macf(c->cross2_c.k1, s->y0[1].left, s->x3[1].left);
       // s->x2[1].left = msubf(c->cross2_c.k1, s->y1[1].left, s->x2[1].left);
       // s->x3[1].left = msubf(c->cross2_c.k2, s->y1[1].left, s->y0[1].left);

       // b2[i].left = s->y1[1].left;

       ///* 1st Order */        
       // s->xn.right = b2[i].right;
       // s->x0[1].right = msubf(c->cross2_c.k0, s->x1[1].right, s->xn.right); 
       // s->y0[1].right =  macf(c->cross2_c.k0, s->x0[1].right, s->x1[1].right);
       // s->x1[1].right = s->x0[1].right;
       // /* 2nd Order */
       // s->y1[1].right =  macf(c->cross2_c.k2, s->y0[1].right, s->x2[1].right);
       // s->x2[1].right =  macf(c->cross2_c.k1, s->y0[1].right, s->x3[1].right);
       // s->x2[1].right = msubf(c->cross2_c.k1, s->y1[1].right, s->x2[1].right);
       // s->x3[1].right = msubf(c->cross2_c.k2, s->y1[1].right, s->y0[1].right);

       // b2[i].right = s->y1[1].right;

    }
    return 0;
}
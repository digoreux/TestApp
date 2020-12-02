#ifndef __FLOATPOINT_H__
#define __FLOATPOINT_H__

#include "fractional.h"
#define KF1 2.823529
#define KF2 1.882353
#define K0  0.0759776172978545212494579946726
#define SSE 0
#define AVX 0
#define NCH 4

#pragma pack(push, 2)
#if SSE
typedef union vector_s {
  __m128 vec; 
  __m128i ivec;
  flt val[4];
} vector_t;
#elif AVX
typedef union vector_s {
  __m256 vec; 
  __m256i ivec;
  flt val[8];
} vector_t;
#else 
typedef union vector_s {
    flt val[4];
} vector_t;
#endif
#pragma pack(pop)

#pragma fp_contract(off)
#pragma fenv_access(on)
#pragma float_control(precise, on)
extern flt negf(flt x);
extern flt absf(flt x);
extern flt addf(flt x, flt y);
extern flt subf(flt x, flt y);
extern flt mulf(flt x, flt y);
extern flt macf(flt x, flt y, flt z);
extern flt msubf(flt x, flt y, flt z);
extern flt divf(flt x, flt y);
extern flt divnr(flt x, flt y);
extern flt pow2(flt n);
extern flt flog2(flt x);
extern flt fpow(flt x, flt y);


static inline vector_t sub2(vector_t x, vector_t y)
{   
    #if SSE
    vector_t r;
    r.vec = _mm_sub_ps(x.vec, y.vec);
    return r;
    #elif AVX
    vector_t r;
    r.vec = _mm256_sub_ps(x.vec, y.vec);
    return r;
    #else
    vector_t r;
    for(uint32_t i = 0; i < NCH; i++)
        r.val[i] = x.val[i] - y.val[i];
    #endif
    return r;
}

static inline vector_t add2(vector_t x, vector_t y)
{   
    #if SSE
    vector_t r;
    r.vec = _mm_add_ps(x.vec, y.vec);
    return r;
    #elif AVX
    vector_t r;
    r.vec = _mm256_add_ps(x.vec, y.vec);
    return r;
    #else
    vector_t r;
    for(uint32_t i = 0; i < NCH; i++)
        r.val[i] = x.val[i] + y.val[i];

    #endif
    return r;
}

static inline vector_t div2(vector_t x, vector_t y)
{   
    #if SSE
    vector_t r;
    r.vec = _mm_div_ps(x.vec, y.vec);
    return r;
    #elif AVX
    vector_t r;
    r.vec = _mm256_div_ps(x.vec, y.vec);
    return r;
    #else
    vector_t r;
    for(uint32_t i = 0; i < NCH; i++)
        r.val[i] = x.val[i] / y.val[i];
    #endif
    return r;
}

static inline vector_t mul2(vector_t x, vector_t y)
{   
    #if SSE
    vector_t r;
    r.vec = _mm_mul_ps(x.vec, y.vec);
    return r;
    #elif AVX
    vector_t r;
    r.vec = _mm256_mul_ps(x.vec, y.vec);
    return r;
    #else
    vector_t r;
    for(uint32_t i = 0; i < NCH; i++)
        r.val[i] = x.val[i] * y.val[i];
    return r;
    #endif
}
 
static inline vector_t fma2(vector_t x, vector_t y, vector_t z) 
{
    #if SSE
    z.vec = _mm_fmadd_ps(x.vec, y.vec, z.vec);
    return z;
    #elif AVX
    vector_t r;
    r.vec = _mm256_fmadd_ps(x.vec, y.vec, z.vec);
    return r;
    #else 
    vector_t r;
    for(uint32_t i = 0; i < NCH; i++)
        r.val[i] =  z.val[i] + (x.val[i] * y.val[i]);
    return r;
    #endif
} 


static inline vector_t fnma2(vector_t x, vector_t y, vector_t z) 
{
    #if SSE
    z.vec = _mm_fnmadd_ps(x.vec, y.vec, z.vec);
    return z;
    #elif AVX
    z.vec = _mm256_fnmadd_ps(x.vec, y.vec, z.vec);
    return z;
    #else 
    vector_t r;
    for(uint32_t i = 0; i < NCH; i++)
        r.val[i] = z.val[i] - (x.val[i] * y.val[i]);
    return r;
    #endif
} 

 
static inline vector_t vpow(vector_t x, vector_t y) 
{
    vector_t r;
    for(uint32_t i = 0; i < NCH; i++)
        r.val[i] = powf(x.val[i], y.val[i]);
    return r;
} 


static inline uint32_t set_vals(vector_t * r, flt x, flt y)
{   
    #if SSE
    r->vec = _mm_set_ps(x, y, x, y);
    return 0;
    #elif AVX
    #else 
    r->val[3] = x;
    r->val[2] = y;
    r->val[1] = x;
    r->val[0] = y;
    return 0;
    #endif
}

static inline vector_t max2(vector_t x, vector_t y)
{   
    #if SSE
    vector_t r;
    r.vec = _mm_max_ps(x.vec, y.vec);
    return r;
    #elif AVX
    #else
    vector_t r;
    for(uint32_t i = 0; i < NCH; i++)
        r.val[i] = fmaxf(x.val[i], y.val[i]);
    return r;
    #endif
}

static inline vector_t cmpgt(vector_t x, vector_t y)
{   
    #if SSE
    vector_t r;
    r.vec = _mm_cmpgt_ps(x.vec, y.vec);
    return r;
    #elif AVX
    #else
    vector_t r;
    for(uint32_t i = 0; i < NCH; i++)
        r.val[i] = (float)(x.val[i] > y.val[i]);
    return r;
    #endif
}

static inline vector_t cmplt(vector_t x, vector_t y)
{   
    #if SSE
    vector_t r;
    r.vec = _mm_cmplt_ps(x.vec, y.vec);
    return r;
    #elif AVX
    #else
    vector_t r;
    for(uint32_t i = 0; i < NCH; i++)
        r.val[i] = (float)(x.val[i] < y.val[i]);
    return r;
    #endif
}

static inline vector_t cmple(vector_t x, vector_t y)
{   
    #if SSE
    vector_t r;
    r.vec = _mm_cmple_ps(x.vec, y.vec);
    return r;
    #elif AVX
    #else
    vector_t r;
    for(uint32_t i = 0; i < NCH; i++)
        r.val[i] = (float)(x.val[i] <= y.val[i]);
    return r;
    #endif
}

static inline vector_t absf2(vector_t x)
{   
    vector_t r;
    r.val[3] = fabsf(x.val[3]);
    r.val[2] = fabsf(x.val[2]);
    r.val[1] = fabsf(x.val[1]);
    r.val[0] = fabsf(x.val[0]);
    return r;
}

static inline vector_t maskload(vector_t  x, vector_t m)
{   
    #if SSE
    vector_t r;
    r.vec = _mm_maskload_ps(x.val, m.ivec);
    return r;
    #elif AVX
    #else 
    vector_t r;
    for(uint32_t i = 0; i < NCH; i++)
        if(m.val[i] ) r.val[i] = x.val[i];
    return r;
    #endif
}

static inline vector_t blendv(vector_t  x, vector_t y, vector_t m)
{   
    #if SSE
    vector_t r;
    r.vec = _mm_blendv_ps(x.vec, y.vec, m.vec);
    return r;
    #elif AVX
    #else 
    for(uint32_t i = 0; i < NCH; i++)
        if(m.val[i])  x.val[i] = y.val[i];
    #endif
    return x;
}

static inline uint32_t set_vals2(vector_t * r, flt a, flt b, flt c, flt d)
{   
    #if SSE
    r->vec = _mm_set_ps(a, b, c, d);
    return 0;
    #elif AVX
    #else 
    r->val[3] = a;
    r->val[2] = b;
    r->val[1] = c;
    r->val[0] = d;
    return 0;
    #endif
}

static inline uint32_t set_val(vector_t * r, flt x)
{   
    #if SSE
    r->vec = _mm_set_ps(x, x, x, x);
    return 0;
    #elif AVX
    r->vec = _mm256_set_ps(x, x, x, x, x, x, x, x);
    return 0;
    #else 
    for(uint32_t i = 0; i < NCH; i++)
        r->val[i] = x;
    return 0;
    #endif
}

static inline uint32_t get_vals(flt * x, vector_t y)
{
    #if SSE
    // xmm r;
    // r.v = y.val;
    // x[0] = r.f[3];
    // x[1] = r.f[2];
    // _mm_store_ps(r.v, y.val);
    return 0;
    #elif AVX
    #else
    for(uint32_t i = 0; i < NCH; i++)
        x[i] = y.val[i];
    return 0;  
    #endif
}
static inline uint32_t get_val(flt * x, vector_t y)
{
    #if SSE
    x[0] = y.val[3];
    x[1] = y.val[2];
    // _mm_store_ps(r.v, y.val);
    return 0;
    #elif AVX
    #else
    *x = y.val[3];
    return 0;
    #endif
}

static inline uint32_t get_vals2(flt * x, vector_t y)
{
    #if SSE
    flt r[4];
    _mm_store_ps(r, y.vec);
    *x = r[3];
    x++;
    *x = r[2];
    return 0;
    #elif AVX
    #else
    *x = y.val[3];
    x++;
    *x = y.val[2];
    return 0;
    #endif
}

static inline uint32_t printv(vector_t x)
{   
    for(uint32_t i = 0; i < NCH; i++)
        printf("[%d]: %f \n",i, x.val[i]);
    return 0;
}

static inline vector_t negf2(vector_t x)
{   
    #if SSE
    vector_t r, m;
    set_val(&m, -1.0f);
    r.vec = _mm_mul_ps(x.vec, m.vec);
    return r;
    #elif AVX
    #else
    vector_t r;
    for(uint32_t i = 0; i < NCH; i++)
        r.val[i] = -x.val[i];
    #endif
    return r;
}

#endif
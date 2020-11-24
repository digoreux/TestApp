#ifndef __FLOATPOINT_H__
#define __FLOATPOINT_H__

#include "fractional.h"
#define NCH 4
#define KF1 2.823529
#define KF2 1.882353
#define K0  0.0759776172978545212494579946726
#define INTR 1

// #pragma pack(push, 2)
#if INTR
typedef union vector_s {
  __m128 vec; 
  flt val[4];
} vector_t;
#else
typedef struct vector_s {
  flt val[4];
} vector_t;
#endif
// #pragma pack(pop)


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

__forceinline static vector_t mul2(vector_t x, vector_t y)
{   
    #if INTR
    y.vec = _mm_mul_ps(x.vec, y.vec);
    return y;
    #else
    // for(uint32_t i = 0; i < NCH; i++)
    //     y.val[i] *= x.val[i];
    y.val[3] *= x.val[3];
    y.val[2] *= x.val[2];
    return y;
    #endif
}
 
__forceinline static vector_t fma2(vector_t x, vector_t y, vector_t z) 
{
    #if INTR
    z.vec = _mm_fmadd_ps(x.vec, y.vec, z.vec);
    return z;
    #else 
    // for(uint32_t i = 0; i < NCH; i++)
        // z.val[i] = macf(x.val[i], y.val[i], z.val[i]);
    // z.val[3] = macf(x.val[3], y.val[3], z.val[3]);
    // z.val[2] = macf(x.val[2], y.val[2], z.val[2]);
    z.val[3] = (x.val[3] * y.val[3]) + z.val[3];
    z.val[2] = (x.val[2] * y.val[2]) + z.val[2];
    return z;
    #endif

} 

__forceinline static vector_t fnma2(vector_t x, vector_t y, vector_t z) 
{
    #if INTR
    z.vec = _mm_fnmadd_ps(x.vec, y.vec, z.vec);
    return z;
    #else 
    // for(uint32_t i = 0; i < NCH; i++)
        // z.val[i] = macf(x.val[i], y.val[i], z.val[i]);
    // z.val[3] = macf(x.val[3], y.val[3], z.val[3]);
    // z.val[2] = macf(x.val[2], y.val[2], z.val[2]);
    z.val[3] = -(x.val[3] * y.val[3]) + z.val[3];
    z.val[2] = -(x.val[2] * y.val[2]) + z.val[2];
    return z;
    #endif

} 

__forceinline static uint32_t set_vals(vector_t * r, flt x, flt y)
{   
    #if INTR
    r->vec = _mm_set_ps(x, y, x, y);
    return 0;
    #else 
    r->val[0] = y;
    r->val[1] = x;
    r->val[2] = y;
    r->val[3] = x;
    return 0;
    #endif
}
__forceinline static uint32_t set_vals2(vector_t * r, flt a, flt b, flt c, flt d)
{   
    #if INTR
    r->vec = _mm_set_ps(a, b, c, d);
    return 0;
    #else 
    r->val[3] = a;
    r->val[2] = b;
    r->val[1] = c;
    r->val[0] = d;
    return 0;
    #endif
}

__forceinline static uint32_t set_val(vector_t * r, flt x)
{   
    #if INTR
    r->vec = _mm_set_ps(x, x, x, x);
    return 0;
    #else 
    for(uint32_t i = 0; i < NCH; i++)
        r->val[i] = x;
    return 0;
    #endif
}

__forceinline static uint32_t get_vals(flt * x, vector_t y)
{
    #if INTR
    // xmm r;
    // r.v = y.val;
    // x[0] = r.f[3];
    // x[1] = r.f[2];
    // _mm_store_ps(r.v, y.val);
    return 0;
    #else
    for(uint32_t i = 0; i < NCH; i++)
        x[i] = y.val[i];
    return 0;  
    #endif
}
__forceinline static uint32_t get_val(flt * x, vector_t y)
{
    #if INTR
    x[0] = y.val[3];
    x[1] = y.val[2];
    // _mm_store_ps(r.v, y.val);
    return 0;
    #else
    *x = y.val[3];
    return 0;
    #endif
}

__forceinline static uint32_t get_vals2(flt * x, vector_t y)
{
    #if INTR
    flt r[4];
    _mm_store_ps(r, y.vec);
    *x = r[3];
    x++;
    *x = r[2];
    return 0;
    #else
    *x = y.val[3];
    x++;
    *x = y.val[2];
    return 0;
    #endif
}


#endif

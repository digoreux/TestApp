#include "floatpoint.h"


inline flt negf(flt x)
{
    return -x;
}

inline flt absf(flt x)
{
    return fabsf(x);
}

inline flt addf(flt x, flt y)
{
    return x + y;
}

inline flt subf(flt x, flt y)
{
    return  x - y;
}

inline flt mulf(flt x, flt y)
{
    return x * y;
}

inline flt macf(flt x, flt y, flt z)
{
    return z + (x * y);
}


inline flt msubf(flt x, flt y, flt z)
{
    return z - (x * y);
}

inline flt divf(flt x, flt y)
{
    return x/y;
}

inline flt divnr(flt x, flt y)
{
    flt N  = (flt)K0 * x;
    flt D  = (flt)K0 * y;
    flt xi = (flt)KF1 - (flt)KF2 * D;
    //printf("ref xi: %f\n", xi);

    for (int i = 0; i < 5; i++)
    {
        xi = xi + xi * (1 - D * xi);
        printf("%d. %f\n", i, xi);
    }

    return N * xi;
}

inline flt pow2(flt n) 
{
    return powf(2.0, n);
}

inline flt flog2(flt x)
{
    return log2f(x);
}

inline flt fpow(flt x, flt y) 
{
    return powf(x, y);
}

inline vector_t mul2(vector_t x, vector_t y)
{   
    #if INTR
    y.val = _mm_mul_ps(x.val, y.val);
    return y;
    #else
    for(uint32_t i = 0; i < NCH; i++)
        y.val[i] *= x.val[i];
    return y;
    #endif
}
 
inline vector_t fma2(vector_t x, vector_t y, vector_t z) 
{
    #if INTR
    z.val = _mm_fmadd_ps(x.val, y.val, z.val);
    return z;
    #else 
    for(uint32_t i = 0; i < NCH; i++)
        z.val[i] = macf(x.val[i], y.val[i], z.val[i]);
    return z;
    #endif

} 

inline uint32_t set_vals(vector_t * r, flt x, flt y)
{   
    #if INTR
    r->val = _mm_set_ps(x, y, x, y);
    return 0;
    #else 
    r->val[0] = 0.0f;
    r->val[1] = 0.0f;
    r->val[2] = x;
    r->val[3] = y;
    return 0;
    #endif
}

inline uint32_t set_val(vector_t * r, flt x)
{   
    #if INTR
    r->val = _mm_set_ps(x, x, x, x);
    return 0;
    #else 
    for(uint32_t i = 0; i < NCH; i++)
        r->val[i] = x;
    return 0;
    #endif
}

inline uint32_t get_vals(flt * x, vector_t y)
{
    #if INTR
    xmm r;
    r.v = y.val;
    x[0] = r.f[3];
    x[1] = r.f[2];
    // _mm_store_ps(r.v, y.val);
    return 0;
    #else
    for(uint32_t i = 0; i < NCH; i++)
        x[i] = y.val[i];
    return 0;
    #endif
}
inline uint32_t get_val(flt * x, vector_t y)
{
    #if INTR
    xmm r;
    r.v = y.val;
    x[0] = r.f[3];
    x[1] = r.f[2];
    // _mm_store_ps(r.v, y.val);
    return 0;
    #else
    *x = y.val[3];
    return 0;
    #endif
}

inline uint32_t get_vals2(flt * x, vector_t y)
{
    #if INTR
    flt r[4];
    _mm_store_ps(r, y.val);
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

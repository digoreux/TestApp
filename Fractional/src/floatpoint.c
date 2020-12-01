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
    flt r; 
    r = x * y;
    return r;
}

inline flt macf(flt x, flt y, flt z)
{   
    flt r;
    r = z + (x * y);
    return r;
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

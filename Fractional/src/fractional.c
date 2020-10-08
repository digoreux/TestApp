#include "fractional.h"

#define Q31 31
#define Q25 25    //div
#define Q26 26    //log

#define K1 94742680   // for Q25
#define K2 63161788

#define KF1 2.823529
#define KF2 1.882353
#define K0  0.0759776172978545212494579946726


void bin(unsigned n)
{
    unsigned i;
    int count = 0;
    for (i = 1 << 7; i > 0; i = i / 2)
    {
        (n & i) ? printf("1") : printf("0");
        count++;
        if (count == 4) printf(" ");

    }

}

void test_fractional(void)
{
    flt x = -0.43, y = 0.3464;
    flt b = 0.456, c = 0.456;
    q31 res;
    q31 qb = float2fixed(b);
    q31 qc = float2fixed(c);
    q31 qx = double2fixed_q(x);
    q31 qy = float2fixed(y);

    res = pow2_q31(qx);
    printf("Result:    %f\n", fixed2float(res));
    printf("Reference: %f\n\n", pow(2.0, x));

    res = pow_q31(qb, qc);
    printf("Result:   %f\n", fixed2float(res));
    printf("Reference %f\n\n", pow(b, c));

    res = log2_q31(qy);
    printf("Result:    %f\n", fixed2double_q(res));
    printf("Reference: %lf\n\n", log2(y));

    res = div_q31(qb, qc);
    printf("Result:    %f\n", fixed2float(res));
    printf("Reference: %f\n\n", b / c);
}

q63 saturate(q63 r) {

    if (r > INT32_MAX) r = INT32_MAX;
    if (r < INT32_MIN) r = INT32_MIN;
    return r;
}

q31 left_shift(q31 x, q31 n)
{
    q31 r = x << n;
    if(x > 0 && r < 0) r =  INT32_MAX;
    if(x < 0 && r > 0) r = INT32_MIN;
    return r;
}

q31 right_shift(q31 x, q31 n)
{
    q31 r = x >> n;
    assert(n < 32);
    x = 0;
    return r;
}

q31 neg_q31(q31 x)
{
    q31 r = (x == INT32_MIN) ? INT32_MAX : -x;
    return r;
}

q31 abs_q31(q31 x)
{
    q31 r;
    if (x == INT32_MIN) 
    {
        r = INT32_MAX;
    }
    else
    {
        r = (x < 0) ? -x : x;
    }
    return r;
}

q31 add_q31(q31 x, q31 y)
{   
    q63 r = x + y;
    r = saturate(r);
    return (q31)r;
}

q31 sub_q31(q31 x, q31 y)
{   
    q63 r = x + y;
    r = saturate(r);
    return (q31)r;
}

q63 add_q63(q63 x, q63 y)
{   
    q63 r;
    r = x + y;
    if (((x > 0) && (y > INT64_MAX - x)) ||
        ((x < 0) && (y < INT64_MIN - x)))
    {
        r = (x < 0) ? INT64_MIN : INT64_MAX;
    }
    return r;
}

q63 sub_q63(q63 x, q63 y)
{   
    q63 r;
    r = x - y;
    if (((x > 0) && (y < INT64_MIN + x)) ||
        ((x < 0) && (y > INT64_MAX + x)))
    {
        r = (x < 0) ? INT64_MIN : INT64_MAX;
    }
    return r;
}

q63 mac_q31(q31 x, q31 y, q63 z)
{   
    z = add_q63(mul_q63(x, y), z);
    return z;
}

q63 msub_q31(q31 x, q31 y, q63 z)
{   
    z = sub_q63(mul_q63(x, y), z);
    return z;
} 

q31 gethigh(q63 x) 
{
    x += (1u << (Q31 - 1));
    return x >> Q31; 
}

q31 mul_q31(q31 x, q31 y)
{   
    q63 acum = mul_q63(x,y);
    q31 r    = gethigh(acum);
    
    return r;
}

q63 mul_q63(q31 x, q31 y)
{   
    q63 r = (q63)x * y;
    
    return r;
}

q31 div_q31(q31 x, q31 y) 
{   

    int64_t xi, xn, N, D, tmp, r;
    q31 one = (1 << Q25);
    
    N = (x >> (Q31 - Q25));
    D = (y >> (Q31 - Q25));
    
    
    xi = mul_q63(K2, D);
    xi >>= Q25;
    xi = sub_q63(K1, xi);

    for (int i = 0; i < 3; i++)
    { 
        xn = mul_q63(xi, D);  
        xn >>= Q25;
        xn = sub_q63(one , xn);
        xn = mul_q63(xi, xn);
        xn >>= Q25;
        xn = add_q63(xi, xn);
        xi = xn;
    }

     r = mul_q63(N, xi);
     r >>= (Q25 - (Q31 - Q25));

    return (q31)r;
}

q31 pow2_q31(q31 x)
{
    q31 i = -(x >> 22);
    q31 idelta = (x & ((1 << 22) - 1));

    q63 k = (pow2lookup[i - 1] - pow2lookup[i]);
    k = (k * idelta) >> 22;
    q31 r = pow2lookup[i] + k;

    printf("Pow2 index: %d\n", i);
    
    return (q31)r;
}

q31 log2_q31(q31 x)  
{
    q31 i = (x >> 22);
    q31 idelta = (x & ((1 << 22) - 1)); 
    
    q63 k = (log2lookup[i + 1] - log2lookup[i]);
    k = (k * idelta) >> 22;
    
   printf("Log2 index: %d\n", i);

    return log2lookup[i] + k;
}

q31 pow_q31(q31 x, q31 y) 
{
    q63 r = mul_q63(y, log2_q31(x));  // x ^ y = 2 ^ (y * log(x))
    r >>= Q31;
    r = pow2_q31(r);
    return (q31)r;
}

q31 float2fixed(flt x)
{
    x *= (1u << Q31);
    x = min(x, INT32_MAX);
    x = max(x, INT32_MIN);
    return (q31)x;
}

flt fixed2float(q31 x)
{
 
    flt r = (flt) x / (flt)(1u << Q31);

    return r;
}

int64_t double2fixed_q(double x)
{
    x *= (1ll << Q26);
    x = min(x, INT32_MAX);
    x = max(x, INT32_MIN);
    return (int64_t)x;
}

double fixed2double_q(q31 x)
{
    double r = (double)x / (double)(1ll << Q26);

    return r;
}


// 1 = log2;  2 = pow2
void gentable(int type)   
{   
    if(type == 1) //log2
    {
        double delta = 1.0 / 512;
        double init = fixed2float(1);
        double val = 0;
        for (int i = 0; i < 512; i++)
        {

            val = log2(init);
            printf("%d. log(%0.10f) = %ld  ", i, init, double2fixed_q(val));
            printf("%0.10f \n", val);
            //printf("%10d, \n" ,double2fixed_q(val));
            init += delta;
        }
    }
    if (type == 2) //pow2
    {
        double val;
        q31 qval;
        for (int i = 0; i < 512; i++)
        {
            val = pow(2.0, ((double)-i / (double)16.0));
            qval = float2fixed(val);
            //printf("%d pow(2, %0.10lf) = %0.10lf  ", i, (double)-i / (double)16.0, val);
            printf("%d,\n", qval);
        }
    }

    
}



flt negf(flt x)
{
    return -x;
}

flt absf(flt x)
{
    return fabs(x);
}

flt addf(flt x, flt y)
{
    flt r = x + y;
    return r;
}

flt subf(flt x, flt y)
{
    flt r = x - y;
    return r;
}

flt mulf(flt x, flt y)
{
    flt r = x * y;
    return r;
}

flt macf(flt x, flt y, flt z)
{
    z += (x * y);
    return z;
}

flt msubf(flt x, flt y, flt z)
{
    z -= (x * y);
    return z;
}

flt divf(flt x, flt y)
{
    flt N = K0 * x;
    flt D = K0 * y;
    flt xi = KF1 - KF2 * D;
    //printf("ref xi: %f\n", xi);

    for (int i = 0; i < 5; i++)
    {
        xi = xi + xi * (1 - D * xi);
        printf("%d. %f\n", i, xi);
    }

    return N * xi;
}

flt pow2(flt n) 
{
    return pow(2.0, n);
}

flt flog2(flt x)
{
    return log2(x);
}


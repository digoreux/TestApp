#include "fractional.h"

#define Q31 31
#define Q30 30
#define Q25 25    //div
#define Q26 26    //log

#define Q25 25

#define K1 94742680   // for Q25
#define K2 63161788

#define KF1 2.823529
#define KF2 1.882353
#define K0  0.0759776172978545212494579946726


void bin(unsigned n)
{
    unsigned i;
    int count = 0;
    for (i = 1 << 31; i > 0; i = i / 2)
    {
        (n & i) ? printf("1") : printf("0");
        count++;
        if (count == 16) {
            printf(" ");
            count = 0;
        }
    }

}

void test_fractional(void)
{

    q31 max = INT32_MAX;
    q31 min = INT32_MIN;

    // printf("\n");
    // printf("neg(MAX) = %d  \n", neg_q31(max));
    // printf("neg(MIN) =  %d \n", neg_q31(min));
    // printf("----------------------\n");
    // printf("abs(MAX) =  %d  \n", abs_q31(max));
    // printf("abs(MIN) =  %d  \n", abs_q31(min));
    // printf("----------------------\n");
    // printf("lshift( 1, 31) =  %d \n", left_shift_q31(1, 31));
    // printf("lshift(-1, 31) = %d  \n", left_shift_q31(-1, 31));
    // printf("----------------------------\n");
    // printf("add(MAX,  10) =  %d\n", add_q31(max, 10));
    // printf("add(MIN, -10) = %d\n", add_q31(min, -10));
    // printf("---------------------------\n");
    // printf("sub(MAX, -10) =  %d\n", sub_q31(max, -10));
    // printf("sub(MIN,  10) = %d\n", sub_q31(min, 10));
    // printf("---------------------------\n");
    // printf("mul(MIN, MIN) =  %d\n", mul_q31(min, min));
    // printf("---------------------------\n");
    // printf("div(MIN, MIN) =  %d\n", div_q31(min, min));
    // printf("div(MIN, MAX) = %d\n", div_q31(min, max));
    // printf("div(MAX, MAX) =  %d\n", div_q31(max, max));
    // printf("div(MAX, MAX) =  %0.8f\n", fixed2float_q31(div_q31(max, max)));
    // printf("---------------------------\n");

    q31 res;
    flt b =  0.23f, c = 0.6f;
    q31 qb = float2fixed_q31(b);
    q31 qc = float2fixed_q31(c);

    flt x = 0.9980468755f, y = 0.0000000005f, z = -31.8750000000f;
    q31 qx = float2fixed_q31(x);
    q31 qy = float2fixed_q31(y);
    q31 qx26 = float2fixed_q(x, 26);
    q31 qy26 = float2fixed_q(y, 26);
    q31 qz26 = float2fixed_q(z, 26);

    // res = div_q31(qb, qc);
    // printf("div(%f, %f)\n", x, y);
    // printf("Result:    %f\n", fixed2float_q31(res));
    // printf("Reference: %f\n\n", b / c);

    res = log2_q31(qy);
    printf("log2(%0.10f)\n", y);
    printf("Result:    %f\n", fixed2float_q(res, 26));
    printf("Reference: %lf\n\n", log2(y));

    res = log2_q31(qx);
    printf("log2(%0.10f)\n", x);
    printf("Result:    %f\n", fixed2float_q(res, 26));
    printf("Reference: %lf\n\n", log2(x));


    res = pow2_q31(qy26);
    printf("pow2(%0.10f)\n", y);
    printf("Result:    %f\n", fixed2float_q31(res));
    printf("Reference: %f\n\n", pow(2.0, y));

    res = pow2_q31(qz26);
    printf("pow2(%0.10f)\n", z);
    printf("Result:    %0.16f\n", fixed2float_q31(res));
    printf("Reference: %0.16f\n\n", pow(2.0, z));

    res = pow_q31(qb, qc);
    printf("pow(%f, %f)\n", b, c);
    printf("Result:   %f\n", fixed2float_q31(res));
    printf("Reference %f\n\n", pow(b, c));

}

q63 saturate(q63 r) 
{
    if (r > INT32_MAX) return INT32_MAX;
    if (r < INT32_MIN) return INT32_MIN;

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
        r = (x < 0) ? neg_q31(x) : x;
    }
    return r;
}



q31 left_shift_q31(q31 x, q31 n)
{
    q31 r = x << n;
    if(x > 0 && r < 0) r = INT32_MAX;
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

q63 left_shift_q63(q63 x, q31 n) 
{
    q63 r = x << n;
    if (x > 0 && r < 0) r = INT64_MAX;
    if (x < 0 && r > 0) r = INT64_MIN;
    return r;
};

q63 right_shift_q63(q63 x, q31 n)
{
    q63 r = x >> n;
    assert(n < 64);
    x = 0;
    return r;
};




q31 add_q31(q31 x, q31 y)
{   
    q63 r = x;
    r += y;
    r = saturate(r);
    return (q31)r;
}

q31 sub_q31(q31 x, q31 y)
{   
    q63 r = x;
    r -= y;
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



union bytes {
    q63 all;
    struct {
        q31 low;
        q31 high;
    } part;
};

q31 gethigh(q63 x)
{   
    union bytes r;
    r.all = x;
    r.all += (1lu << (Q31 - 1));
    return r.part.high;
}
// q31 gethigh(q63 x)
// {   
//     q63 r = x;
//     r += (1lu << (Q31 - 1));
//     r = right_shift_q63(r, 32);
//     return (q31)r;
// }

q31 getlow(q63 x)
{
    union bytes r;
    r.all = x;
    return r.part.low;
}


q31 mul_q31(q31 x, q31 y)
{   
    if (x == INT32_MIN && y == INT32_MIN) return INT32_MAX;
    q63 acum = mul_q63(x,y);
    q31 r    = gethigh(acum);
    
    return r;
}

q63 mul_q63(q31 x, q31 y)
{   
    q63 r = x;
    r *= y;
    // r = left_shift_q63(r, 1);
    return r;
}


q63 mac_q31(q31 x, q31 y, q63 z)
{   
    z = add_q63(mul_q63(x, y), z);
    return z;
}

q63 msub_q31(q31 x, q31 y, q63 z)
{   
    z = sub_q63(z, mul_q63(x, y));
    return z;
} 


q31 div_q31(q31 x, q31 y) 
{   
    if (x == INT32_MIN && y == INT32_MIN) return INT32_MAX;

    q64 xi, xn, r, N, D;
    q32 one = (1 << Q25);       // 1 in Q6.25 format
    
    N = (x >> (Q31 - Q25));     
    D = (y >> (Q31 - Q25));
    
    
    xi = K2 * D;               
    xi = right_shift_q63(xi, 25);                
    xi = sub_q63(K1, xi);

    for (int i = 0; i < 3; i++)
    { 
        xn = xi * D;  
        xn = right_shift_q63(xn, 25);
        xn = sub_q63(one , xn);
        xn = xi * xn;
        xn = right_shift_q63(xn, 25);
        xn = add_q63(xi, xn);
        xi = xn;
    }
     r = N * xi;
     r = right_shift_q63(r, (Q25 - (Q31 - Q25)));  

    return (q31)r;
}


q31 pow2_q31(q31 x)
{
    q31 i = -(x >> 22);
    q31 idelta = (x & ((1 << 22) - 1));

    q63 k = (pow2lookup[i - 1] - pow2lookup[i]);
    k = (k * idelta) >> 22;
    q31 r = pow2lookup[i] + (q31)k;

    //printf("Pow2 index: %d\n", i);
    
    return (q31)r;
}

q31 log2_q31(q31 x)  
{
    q31 i = (x >> 22);
    q31 idelta = (x & ((1 << 22) - 1)); 
    
    q63 k = (log2lookup[i + 1] - log2lookup[i]);
    k = (k * idelta) >> 22;
    
   //printf("Log2 index: %d\n", i);

    return log2lookup[i] + (q31)k;
}


q31 pow_q31(q31 x, q31 y) 
{
    q63 r = y;
    r *= log2_q31(x);  
    r = right_shift_q63(r, 31);   // Q37.26
    r = pow2_q31((q31)r);
    return (q31)r;
}




q31 float2fixed_q31(flt x)
{
    if (x >= 1.0)  return INT32_MAX;
    if (x <= -1.0) return INT32_MIN;

    x *= (1u << 31);

    return (q31)x;
}

flt fixed2float_q31(q31 x)
{
    flt r = (flt) x / (flt)(1ul << 31);
    return r;
}




q31 double2fixed_q(double x, size_t Q)
{
    x *= (1ll << Q);
    x = min(x, INT32_MAX);
    x = max(x, INT32_MIN);
    return (q31)x;
}

double fixed2double_q(q31 x, size_t Q)
{
    double r = (double)x / (double)(1ll << Q);
    return r;
}

q32 float2fixed_q(flt x, size_t Q)
{
    x *= (1ll << Q);
    x = min(x, INT32_MAX);
    x = max(x, INT32_MIN);
    return (q32)x;
}

flt fixed2float_q(q32 x, size_t Q)
{
    flt r = (flt)x / (flt)(1ll << Q);
    return r;
}


void gen_table(int type)   
{   
    if(type == 1) //log2
    {
        double delta = 1.0 / 512;
        double init = fixed2float_q31(1);
        double val = 0;
        for (int i = 0; i < 512; i++)
        {

            val = log2(init);
            printf("%d. log(%0.10f) = %d  ", i, init, double2fixed_q(val, 26));
            printf("%0.10f \n", val);
            //printf("%10d, \n" ,double2fixed_q(val, 26));
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
            qval = float2fixed_q31((float)val);
            printf("%d pow(2, %0.10lf) = %0.10lf  ", i, (double)-i / (double)16.0, val);
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
    return fabsf(x);
}

flt addf(flt x, flt y)
{
    return x + y;
}

flt subf(flt x, flt y)
{
    return  x - y;
}

flt mulf(flt x, flt y)
{
    return x * y;
}

flt macf(flt x, flt y, flt z)
{
    return z + (x * y);
}

flt msubf(flt x, flt y, flt z)
{
    return z - (x * y);
}

flt divf(flt x, flt y)
{
    return x/y;
}

flt divnr(flt x, flt y)
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

flt pow2(flt n) 
{
    return powf(2.0, n);
}

flt flog2(flt x)
{
    return log2f(x);
}

flt fpow(flt x, flt y) {
    return powf(x, y);
}

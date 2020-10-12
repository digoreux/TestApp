#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int main(int argc, const char * argv[]) {
        
    int size  = sizeof(float) * 512;
    
    float *buffer  = malloc(size);

    double b0, b1, b2, a0, a1, a2;
    float x0=0, x1=0, x2=0, y1=0, y2=0;
    float acc = 0;

    FILE *in  = fopen("./audio/input.raw", "rb");
    FILE *out = fopen("./audio/output.raw", "wb");
    
    //double dbGain = -12;
    double frequency = 240;
    double sample_rate = 48000;
    double Q = 0.5;

    //double A = pow(10, dbGain / 40);
    double omega = 2 * M_PI * frequency / sample_rate;
    double sn = sin(omega);
    double cs = cos(omega);
    double alpha = sn / (2 * Q);

    b0 = (1.0 - cs) / 2.0;
    b1 =  1.0 - cs;
    b2 = (1.0 - cs) / 2.0;
    a0 =  1.0 + alpha;
    a1 = -2.0 * cs;
    a2 =  1.0 - alpha;

    a1 /= a0;
    a2 /= a0;
    b0 /= a0;
    b1 /= a0;
    b2 /= a0;

    while(!feof(in))
    {
        fread(buffer, size, 1, in);

        for (int i = 0; i < 512; i++)
        {
            x0  = buffer[i];

            acc = (b0 * x0) + 
                  (b1 * x1) +
                  (b2 * x2) -
                  (a1 * y1) -
                  (a2 * y2);

            x2 = x1;
            x1 = x0;

            y2 = y1;
            y1 = acc;
        
            buffer[i] = acc;
        }
        fwrite(buffer, size, 1, out);
    }
    
    printf("Done!\n");
    return 0;
}



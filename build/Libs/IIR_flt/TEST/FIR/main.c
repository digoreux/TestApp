#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float coeffs[128];

int main(int argc, const char * argv[]) {
        
    int size  = sizeof(float) * 512;
    int csize = sizeof(float) * 128;
    
    float *buffer  = malloc(size);
    float *cbuffer = malloc(csize);
    
    memset(cbuffer, 0, csize);
    int index = -1;
    
    FILE *in =  fopen("./audio/input.raw", "rb");
    FILE *out = fopen("./audio/output.raw", "wb");
    
    while(!feof(in))
    {
        fread(buffer, size, 1, in);
        
        for(int i = 0; i < 512; i++)
        {
            index = i & 127;
            cbuffer[index] = buffer[i];
            float acc = 0;
            for(int j = 0; j <= 128; j++) {
                acc += cbuffer[(index + j) & 127] * coeffs[j];
            }
            buffer[i] = acc;
        }
        fwrite(buffer, size, 1, out);
    }
    
    printf("Done!\n");
    return 0;
}


float coeffs[128] = {
    1.051344754458186e-03,  1.062871391586219e-03,  1.089289907005056e-03,  1.130712014714360e-03,
    1.187207306709540e-03,  1.258802780145485e-03,  1.345482489958735e-03,  1.447187328645190e-03,
    1.563814934494629e-03,  1.695219729181679e-03,  1.841213085206827e-03,  2.001563623271961e-03,
    2.175997639263956e-03,  2.364199660108559e-03,  2.565813127346379e-03,  2.780441206874715e-03,
    3.007647722894491e-03,  3.246958213702085e-03,  3.497861106572692e-03,  3.759809008596309e-03,
    4.032220109950821e-03,  4.314479695730200e-03,  4.605941762090827e-03,  4.905930732136483e-03,
    5.213743266633868e-03,  5.528650164336783e-03,  5.849898346399159e-03,  6.176712919076270e-03,
    6.508299308650521e-03,  6.843845462273942e-03,  7.182524108195124e-03,  7.523495068634242e-03,
    7.865907618386873e-03,  8.208902882076317e-03,  8.551616262835253e-03,  8.893179895081878e-03,
    9.232725113962885e-03,  9.569384933966876e-03,  9.902296529166715e-03,  1.023060370752823e-02,
    1.055345937172618e-02,  1.087002795893563e-02,  1.117948785211871e-02,  1.148103375540222e-02,
    1.177387902624133e-02,  1.205725795718737e-02,  1.233042800022415e-02,  1.259267192680616e-02,
    1.284329991692288e-02,  1.308165157072661e-02,  1.330709783649430e-02,  1.351904284894819e-02,
    1.371692567223301e-02,  1.390022194213913e-02,  1.406844540247053e-02,  1.422114933078203e-02,
    1.435792784905175e-02,  1.447841711521052e-02,  1.458229639181873e-02,  1.466928898856260e-02,
    1.473916307563342e-02,  1.479173236545498e-02,  1.482685666063401e-02,  1.484444226642533e-02,
    1.484444226642533e-02,  1.482685666063401e-02,  1.479173236545498e-02,  1.473916307563342e-02,
    1.466928898856260e-02,  1.458229639181873e-02,  1.447841711521052e-02,  1.435792784905175e-02,
    1.422114933078203e-02,  1.406844540247053e-02,  1.390022194213913e-02,  1.371692567223301e-02,
    1.351904284894819e-02,  1.330709783649430e-02,  1.308165157072661e-02,  1.284329991692288e-02,
    1.259267192680616e-02,  1.233042800022415e-02,  1.205725795718737e-02,  1.177387902624133e-02,
    1.148103375540222e-02,  1.117948785211871e-02,  1.087002795893563e-02,  1.055345937172618e-02,
    1.023060370752824e-02,  9.902296529166717e-03,  9.569384933966880e-03,  9.232725113962885e-03,
    8.893179895081883e-03,  8.551616262835258e-03,  8.208902882076314e-03,  7.865907618386878e-03,
    7.523495068634244e-03,  7.182524108195128e-03,  6.843845462273943e-03,  6.508299308650521e-03,
    6.176712919076269e-03,  5.849898346399157e-03,  5.528650164336788e-03,  5.213743266633870e-03,
    4.905930732136478e-03,  4.605941762090828e-03,  4.314479695730200e-03,  4.032220109950821e-03,
    3.759809008596313e-03,  3.497861106572696e-03,  3.246958213702084e-03,  3.007647722894493e-03,
    2.780441206874716e-03,  2.565813127346380e-03,  2.364199660108562e-03,  2.175997639263955e-03,
    2.001563623271961e-03,  1.841213085206829e-03,  1.695219729181680e-03,  1.563814934494630e-03,
    1.447187328645192e-03,  1.345482489958735e-03,  1.258802780145485e-03,  1.187207306709540e-03,
    1.130712014714361e-03,  1.089289907005057e-03,  1.062871391586219e-03,  1.051344754458186e-03
};

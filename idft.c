#include <stdio.h>
#include <math.h>

#define N 8
#define PI 3.14

float XR[N] = {8,0,0,0,0,0,0,0};
float XI[N] = {0,0,0,0,0,0,0,0};
float x[N];

int main() {
    int n, k;
    float angle, real_sum, imag_sum;

    for(n=0;n<N;n++){
        x[n]=0;
        for(k=0;k<N;k++){
            float angle = 2*PI*k*n / N;
            x[n] += XR[k]*cos(angle) - XI[k]*sin(angle); 
        }
        x[n] /= N;
    }
    for(n=0;n<N;n++) printf("x[%d] = %f\n", n, x[n]);
    return 0;
}
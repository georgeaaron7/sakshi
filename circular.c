/* Circular convolution of two length-N sequences via DFT: Y = IDFT( DFT(x)*DFT(h) ) */
#include <stdio.h>
#include <math.h>

#define N 8
#define PI 3.14f

void dft(float *in, int N, float *XR, float *XI){
    int k,n;
    for(k=0;k<N;k++){
        XR[k]=0; XI[k]=0;
        for(n=0;n<N;n++){
            float angle = 2*PI*n*k / N;
            XR[k] += in[n]*cos(angle);
            XI[k] -= in[n]*sin(angle);
        }
    }
}
void idft(float *XR, float *XI, int N, float *out){
    int n,k;
    for(n=0;n<N;n++){
        out[n]=0;
        for(k=0;k<N;k++){
            float angle = 2*PI*k*n / N;
            out[n] += XR[k]*cos(angle) - XI[k]*sin(angle);
        }
        out[n] /= N;
    }
}

int main(void){
    int i;
    float x[N] = {1,2,3,4,0,0,0,0}; /* zero-pad or not depending on desired circular length */
    float h[N] = {1,1,0,0,0,0,0,0};
    float XR[N], XI[N], HR[N], HI[N], Y[N];

    dft(x,N,XR,XI);
    dft(h,N,HR,HI);

    for(i=0;i<N;i++){
        float real = XR[i]*HR[i] - XI[i]*HI[i];
        float imag = XR[i]*HI[i] + XI[i]*HR[i];
        XR[i] = real; XI[i] = imag;
    }

    idft(XR,XI,N,Y);

    for(i=0;i<N;i++) printf("circular conv y[%d]=%f\n", i, Y[i]);
    return 0;
}

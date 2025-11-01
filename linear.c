#include <stdio.h>
#include <math.h>

#define L 5
#define M 4
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
    int N = L + M - 1;
    float x[N]; float h[N];
    float XR[N], XI[N], YR[N], YI[N];
    float y[N];
    int i;

    float x_in[L] = {1,2,3,4,5};
    float h_in[M] = {1,1,1,1};
    for(i=0;i<N;i++){
        x[i] = (i < L) ? x_in[i] : 0;
        h[i] = (i < M) ? h_in[i] : 0;
    }

    dft(x, N, XR, XI);
    dft(h, N, YR, YI);

    for(i=0;i<N;i++){
        float real = XR[i]*YR[i] - XI[i]*YI[i];
        float imag = XR[i]*YI[i] + XI[i]*YR[i];
        XR[i] = real; XI[i] = imag;
    }

    idft(XR, XI, N, y);

    for(i=0;i<N;i++) printf("linear conv y[%d]=%f\n", i, y[i]);
    return 0;
}

#include <stdio.h>
#include <math.h>

float x[8] = {1,1,2,0,1,2,0,1};
float XR[8];
float XI[8];
float amplitude[8];
float phase[8];
int N = 8;

int main(void) {
    int i, j;

    
    for (j = 0; j < N; j++) {
        XR[j] = 0;
        XI[j] = 0;
        for (i = 0; i < N; i++) {
            XR[j] += x[i] * cos((2 * 3.14 * i * j) / N);
            XI[j] -= x[i] * sin((2 * 3.14 * i * j) / N);
        }
        printf("X[%d] = %f + i%f\n", j, XR[j], XI[j]);
    }

    
    for (i = 0; i < N; i++) {
        amplitude[i] = sqrt((XR[i] * XR[i]) + (XI[i] * XI[i]));
        printf("Amplitude[%d] = %f\n", i, amplitude[i]);
    }

    for (i = 0; i < N; i++) {
        phase[i] = atan2(XI[i], XR[i]) * 180 / 3.14; 
        printf("Phase[%d] = %f degrees\n", i, phase[i]);
    }

    return 0;
}
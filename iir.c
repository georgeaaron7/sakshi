#include <stdio.h>
#include <math.h>

#define N 100
#define NUM_SECTIONS 1
#define PI 3.14f


float b[NUM_SECTIONS][3] = { {0.0, 0.0214501370969255, 0.0} };
float a[NUM_SECTIONS][3] = { {1.0, -1.83233418096295, 0.859404279941073} };

float x[N], y[N];
float XR[N], XI[N], m1[N], m2[N];
float w[NUM_SECTIONS][2] = {0}; /* delay elements per section */

int main(void) {
    int i, j;

    /* Input signal = 2 Hz + 30 Hz */
    for (i = 0; i < N; i++)
        x[i] = sin(2 * PI * 2 * i / N) + sin(2 * PI * 30 * i / N);

    /* DFT before filtering */
    for (j = 0; j < N; j++) {
        XR[j] = XI[j] = 0;
        for (i = 0; i < N; i++) {
            float angle = 2 * PI * i * j / N;
            XR[j] += x[i] * cos(angle);
            XI[j] -= x[i] * sin(angle);
        }
        m1[j] = sqrt(XR[j]*XR[j] + XI[j]*XI[j]) / N;
    }

    /* IIR filtering (Direct Form II Transposed) */
    for (i = 0; i < N; i++) {
        float input = x[i];
        float yn = 0;
        for (int s = 0; s < NUM_SECTIONS; s++) {
            float wn = input - a[s][1]*w[s][0] - a[s][2]*w[s][1];
            yn = b[s][0]*wn + b[s][1]*w[s][0] + b[s][2]*w[s][1];
            w[s][1] = w[s][0];
            w[s][0] = wn;
            input = yn; /* feed output to next section */
        }
        y[i] = yn;
    }

    /* DFT after filtering */
    for (j = 0; j < N; j++) {
        XR[j] = XI[j] = 0;
        for (i = 0; i < N; i++) {
            float angle = 2 * PI * i * j / N;
            XR[j] += y[i] * cos(angle);
            XI[j] -= y[i] * sin(angle);
        }
        m2[j] = sqrt(XR[j]*XR[j] + XI[j]*XI[j]) / N;
    }

    return 0;
}

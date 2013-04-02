#include <stdio.h>

void cfun(const double *indatav, size_t size, double *outdatav) {
    const double * indata = (double *) indatav;
    double * outdata = (double *) outdatav;
    size_t i;

    for (i = 0; i < size; ++i) {
        outdata[i] = indata[i] * 2.0;
    }
}

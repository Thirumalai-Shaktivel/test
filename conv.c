#include <stdio.h>

struct _lfortran_single_complex {
    float re, im;
};

struct _lfortran_double_complex {
    double re, im;
};

typedef struct _lfortran_single_complex float_complex_t;
typedef struct _lfortran_double_complex double_complex_t;


void cf(float_complex_t x)
{
    printf("f32: %.15f %.15f\n", x.re, x.im);
}

void zf(double_complex_t x)
{
    printf("f64: %.15f %.15f\n", x.re, x.im);
}

int main() {
    float_complex_t f;
    double_complex_t d;
    f.re = 3.5;
    f.im = 4.5;
    cf(f);

    d.re = 3.5;
    d.im = 4.5;
    zf(d);

    return 0;
}

#include <stdio.h>
#include <complex.h>

int main(void)
{
    float complex a = 1 + 2 * I, b = 3 + 4 * I;
    float complex c = a * b;
    printf("(%f, %f)\n", creal(c), cimag(c));

    return 0;
}
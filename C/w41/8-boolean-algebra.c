#include <stdio.h>
#include <stdint.h>

#define EXPRESSION (3 * j / k - 2)
#define SET_VARIABLES \
    {                 \
        i = 3;        \
        j = 2;        \
        k = 0;        \
    }

int main(void)
{
    int i = 0, j = 7, k = 6; // k = 4
    char a = 'A', b = '3';
    int32_t x, y = 3;
    uint32_t z = 32;
    double d;

    x = EXPRESSION;
    printf("A) %d\n", x); // A) 1

    // j = 0111, k = 0110
    // j << 1 = 1110, k >> 1 = 0011
    y -= ((j << 1) & (k >> 1)); // y -= 0010
    printf("B) %d\n", y);       // B) 1

    // k + b = 6 + '3' --> 6 + 51
    x = (!(i && k) || !k) ? k + b : j + b;
    printf("C) %d\n", x); // C) 57

    // 'A' + 7 = 72
    a += (b + k--) % 10;
    printf("D) %d\n", a); // D) 72

    // 700 / 4 / 3
    d = (double)j * 100 / --k / 3;
    printf("E) %f\n", d); // E) 58,333...

    // (k < j < 0)
    x = -1;
    y = (x < z) ? (k < j < 0) : (b >= a < i);
    printf("F) %d\n", y); // F) 0

    printf("x is %d, z is %d\n", x, z);
    d = x / z;
    double test = (double)x / z; // Cast one int to make division work - note: (double)(x / z) will NOT work
    printf("G) %f\n", d);        // G) -0,03125 (Actual: 134217727.000000)
    printf("Test) %f\n", test);

    i = 4;
    j = -1;
    k = 0;

    x = i && j && k;             // 0
    y = i || j && k;             // 1
    printf("H) %d, %d\n", x, y); // H) 0, 1

    x = i && j || k;             // 1
    y = i || j || k;             // 1
    printf("I) %d, %d\n", x, y); // I) 1, 1

    SET_VARIABLES;                             // i = 3, j = 2, k = 0;
    y = ++i || ++j && k++;                     // 1
    printf("J) %d, %d, %d, %d\n", i, j, k, y); // J) 4, 3, 1, 1 (Actual: 4,2,0,1)
    // Note: If the left-hand side of || is non-zero, the right-hand side is not
    // evaluated (short-circuit!).

    SET_VARIABLES;                             // i = 3, j = 2, k = 0;
    y = (123 && --i) ? k++ : ++k;              // 0
    printf("K) %d, %d, %d, %d\n", i, j, k, y); // K) 2, 2, 1, 0

    SET_VARIABLES;                             // i = 3, j = 2, k = 0;
    y = ++i && ++j && k++;                     // 4 && 3 && 0 (post-increment k to 1)
    printf("L) %d, %d, %d, %d\n", i, j, k, y); // L) 4, 3, 1, 0

    i = 4;
    j = --i;                            // 3
    k = i--;                            // k = 3, i = 2
    printf("M) %d, %d, %d\n", i, j, k); // M) 2, 3, 3

    i = -1; // 1
    j = 1;  // -1

    i &&printf("N) Hello World!\n") && --j; // N) Hello World!

    ++i &&j &&printf("O) Hello World!\n"); // (no output)

    i || printf("P) Hello World!\n") || j++; // P) Hello World!
    // j does not increment, short-circuit

    j-- || !printf("Q) Hello World!\n") || i++; // Q) Hello World!
    // j = 0 initially, so the printf runs, then j decrements. i++ runs (!true || i++)

    printf("R) %d, %d\n", i, j); // 1, -1

    return 0;
}

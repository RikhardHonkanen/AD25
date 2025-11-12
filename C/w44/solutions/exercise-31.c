/**
 * @file exercise-31.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a function to count the digits of a given integer number using recursion; E.g. 456 has 3 digits
 *        Make a function to calculate X power Y; i.e (X^Y) using recursion; E.g. 2^3 = 8. X and Y are two integers.
 *
 * @version 0.1
 * @date 2021-03-05
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <stdio.h>
#include <stdint.h>

static uint8_t get_digits(int num)
{
    int a = (num < 0) ? -num : num;
    return (a < 10) ? 1 : (1 + get_digits(a / 10));
}

static float power(int base, int exp)
{
    int temp = (exp < 0) ? -exp : exp;
    float result = (temp == 0) ? 1 : (base * power(base, temp - 1));
    return (exp < 0) ? (1 / result) : result;
}

int main(void)
{
    int x, y;

    x = 1234;
    (void)printf("%d has %u digit(s)\n", x, get_digits(x));

    x = -1234;
    (void)printf("%d has %u digit(s)\n", x, get_digits(x));

    x = 2;
    y = 3;
    (void)printf("%d^%d = %g\n", x, y, power(x, y));

    x = -2;
    y = 3;
    (void)printf("%d^%d = %g\n", x, y, power(x, y));

    x = 2;
    y = -3;
    (void)printf("%d^%d = %g\n", x, y, power(x, y));

    return 0;
}

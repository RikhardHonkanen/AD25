/**
 * @file 31.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Make a function to count number of digits of a given integer number using recursion.
 * For example: 456 has 3 digits
 * Make a function to calculate X^Y using recursion. X and Y are two integers.
 * For example: 2^3 = 8.
 * @version 0.1
 * @date 2025-10-29
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>

int count_digits(int x);
int count_digits_recursively(int x);
float calc_pwr_recursively(int base, int exp);

int count_digits(int x)
{
    int tot = 0;
    while (x)
    {
        tot++;
        x /= 10;
    }
    return tot;
};
int count_digits_recursively(int x)
{
    if (0 == x)
    {
        return 0;
    }
    else
    {
        return count_digits_recursively(x / 10) + 1U;
    }
};
float calc_pwr_recursively(int base, int exp)
{
    if (exp == 0)
    {
        return 1;
    }
    else if (exp < 0)
    {
        return 1.0f / calc_pwr_recursively(base, -exp);
    }
    else
    {
        return base * calc_pwr_recursively(base, exp - 1);
    }
};

int main(void)
{
    int test1 = 4506;
    int base = 2;
    int exp = -4;

    (void)printf("%d\n", count_digits(test1));
    (void)printf("%d\n", count_digits_recursively(test1));
    (void)printf("%f\n", calc_pwr_recursively(base, exp));

    return 0;
};
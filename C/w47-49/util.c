#include "util.h"
#include <stdio.h>
#include <string.h>

/**
 * @brief Get the greatest common divisor of two ints
 *
 * @param a
 * @param b
 * @return int
 */
uint32_t get_greatest_common_divisor(uint16_t a, uint16_t b)
{
    // If a < b. Check all ints up to a/2. Then also check a. Return largest common divisor.
    uint32_t gcd = 1; // Return base case if no larger divisor found.
    uint32_t smallest = (b < a) ? b : a;
    int check = 0;
    if (smallest == b)
    {
        if (a % b == 0)
        {
            gcd = b;
            check = 1;
        }
    }
    else if (smallest == a)
    {
        if (b % a == 0)
        {
            gcd = a;
            check = 1;
        }
    }

    if (check == 0)
    {
        for (size_t i = smallest; i >= 2; i--)
        {
            if (a % i == 0 && b % i == 0)
            {
                gcd = i;
                break;
            }
        }
    }

    return gcd;
}

/**
 * @brief Get the least common multiple of two ints
 *
 * @param a
 * @param b
 * @return int
 */

uint32_t get_least_common_multiple(uint16_t a, uint16_t b)
{
    // If b > a. Check if b divisible by a. If not check if 2b is divisible by a. And so on.
    uint16_t biggest = (b > a) ? b : a;
    uint16_t smallest = (b < a) ? b : a;
    uint32_t lcm = 0;

    for (size_t i = biggest; i <= (a * b); i += biggest)
    {
        if (i % smallest == 0)
        {
            lcm = i;
            break;
        }
    }

    return lcm;
}

int comp(const void *a, const void *b)
{
    // If a is smaller, positive value will be returned
    return (*(char *)a - *(char *)b);
}

/**
 * @brief Sorts a string in alphabetical order
 *
 * @param s
 */
char *sort_string(char *s)
{
    // Sort the array using qsort
    char str[] = "jsaNSHosAlPWnSJq";

    qsort(str, strlen(str), sizeof(char), comp);

    return str;
}
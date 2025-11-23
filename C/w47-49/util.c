#include "util.h"
#include <stdio.h>
#include <ctype.h>
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
    // If a < b. Check all ints up to a/2. First also check a. Return largest common divisor.
    uint32_t gcd = 1; // Return base case if no larger divisor found.
    uint32_t smallest = (b < a) ? b : a;
    if (smallest == b && a % b == 0)
    {
        gcd = b;
    }
    else if (smallest == a && b % a == 0)
    {
        gcd = a;
    }
    else
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

// Case-insensitive comp function
int comp(const void *a, const void *b)
{
    char c1 = *(const char *)a;
    char c2 = *(const char *)b;

    char l1 = tolower(c1);
    char l2 = tolower(c2);

    if (l1 != l2)
        return l1 - l2;

    // If same letter, uppercase should come first
    return c1 - c2;
}
/**
 * @brief Sorts a string in alphabetical order
 *
 * @param s
 */
char *sort_string(char *s)
{
    qsort(s, strlen(s), sizeof(char), comp);
    printf("Sorted string: %s\n", s);
    return s;
}

/** NOTES: actual common implementations for gcd & lcm
 *
uint32_t get_greatest_common_divisor(uint32_t a, uint32_t b)
{
    while (b != 0) {
        uint32_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}
uint32_t get_least_common_multiple(uint32_t a, uint32_t b)
{
    return (a / get_greatest_common_divisor(a, b)) * b;
}
*/
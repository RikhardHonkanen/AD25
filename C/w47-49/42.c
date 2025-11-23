/**
 * @file 42.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Make a utility module to provide the following functionalities:
 * A function to get the greatest common divisor (gcd) of two unsigned integers greater than 0.
 * Example: the gcd of 8 and 12 is 4.
 * A function to get the least common multiple(lcm) of two unsigned integers greater than 0.
 * Example: the lcm of 4 and 6 is 12.
 * A function to sort a string in alphabetical order.
 * Example:
 * String: jsaNSHosAlPWnSJq
 * Sorted String: AaHJjlNnoPqSSssW
 * Make a test program to test the functions using assertion.
 * @version 0.1
 * @date 2025-11-20
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "util.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

int main(void)
{
    // Test cases
    assert(3 == get_greatest_common_divisor(9, 12));
    assert(6 == get_greatest_common_divisor(6, 12));
    assert(2 == get_greatest_common_divisor(10, 12));
    assert(1 == get_greatest_common_divisor(11, 12));
    assert(12 == get_greatest_common_divisor(12, 12));

    assert(12 == get_least_common_multiple(4, 6));
    assert(30 == get_least_common_multiple(10, 3));
    assert(42 == get_least_common_multiple(6, 14));

    char s[] = "jsaNSHosAlPWnSJq";
    assert(strcmp(sort_string(s), "AaHJjlNnoPqSSssW") == 0);

    return 0;
};
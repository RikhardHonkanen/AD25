/**
 * @file 29.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Make a function to print all prime numbers in the range of m and n to the ouput.
 * m and n are two unsigned integers.
 * @version 0.1
 * @date 2025-10-28
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <stdint.h>

void print_primes_in_range(uint32_t m, uint32_t n);
int check_if_prime(uint32_t num);

void print_primes_in_range(uint32_t m, uint32_t n)
{
    (void)printf("Primes in range: ");
    for (uint32_t i = m; i <= n; i++)
    {
        if (i % 2 == 0)
        {
            continue;
        }
        if (check_if_prime(i))
        {
            (void)printf("%d, ", i);
        }
    }
    (void)printf("\n");
};
int check_if_prime(uint32_t num)
{
    int is_prime = 1;
    for (uint32_t i = 3; i * i <= num; i += 2) // Check divisibility from 3 to the square root of num
    {
        if (num % i == 0)
        {
            is_prime = 0;
        }
    }
    return is_prime;
};
int main(void)
{
    uint32_t m, n;
    (void)printf("Enter lower bound: ");
    scanf(" %d", &m);
    (void)printf("Enter upper bound: ");
    scanf(" %d", &n);
    print_primes_in_range(m, n);

    return 0;
};
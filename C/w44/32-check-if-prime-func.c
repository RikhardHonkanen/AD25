/**
 * @file 32.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Make a program to get a positive integer as an argument from the command line and then check if the number is prime or not.
 * @version 0.1
 * @date 2025-11-02
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

bool check_if_prime(uint32_t num);

bool check_if_prime(uint32_t num)
{
    int is_prime = 1;
    if (!(num == 2) && num % 2 == 0)
    {
        is_prime = 0;
    }
    else
    {
        for (uint32_t i = 3; i * i <= num; i += 2) // Check divisibility from 3 to the square root of num, skipping evens
        {
            if (num % i == 0)
            {
                is_prime = 0;
            }
        }
    }

    return is_prime;
};

int main(int argv, char *argc[])
{
    char *arg = argc[1];
    int num = atoi(arg);
    (void)printf("%d %s", num, check_if_prime(num) ? "is prime.\n" : "is not prime.\n");

    return 0;
};
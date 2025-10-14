/**
 * @file 12.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Read an integer number in the range of 2 and 10000 and check if the number is a prime number or not
 * @version 0.1
 * @date 2025-10-14
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>

int main(void)
{
    int num = 0;

    do
    {
        (void)printf("\nEnter a number between 2 and 10000: ");
        (void)scanf(" %d", &num);
    } while (num < 2 || num > 10000);

    // Check divisibility from 2 to the square root of num
    for (int i = 2; i * i <= num; i++)
    {
        if (num % i == 0)
        {
            (void)printf("\n%d is not prime (divisible by %d): ", num, i);
            return 0;
        }
    }

    (void)printf("\n%d is prime.", num);

    return 0;
};
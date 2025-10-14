/**
 * @file 9.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Read a positive integer number from the standard input,
 * then sum all the even numbers from 0 to the entered number and print it to the output.
 * @version 0.1
 * @date 2025-10-14
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <stdio.h>

int main(void)
{
    int num;

    (void)printf("Enter a positive number: ");

    if (1 == scanf("%d", &num))
    {
        int output = 0;
        for (int i = 1; i <= num; i++)
        {
            output += i;
        }
        printf("Sum is: %d", output);
    };

    return 0;
}
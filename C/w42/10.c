/**
 * @file 10.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Print all the numbers in the range of 0 and 1000 whose last digit is 7
 * @version 0.1
 * @date 2025-10-14
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>

int main(void)
{
    for (int i = 7; i <= 1000; i += 10)
    {
        printf("%d ", i);
    }
};
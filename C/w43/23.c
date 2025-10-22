/**
 * @file 23.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Program to fill a 10-element int array with random numbers in the range of 0 and 99, then print the array to the output.
 * Sort the array ascending(from the smallest to the largest) and then print the sorted array to the output.
 * To fill, print and sort the array use only pointers.
 * @version 0.1
 * @date 2025-10-21
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <time.h>

int main(void)
{
    srand(time(NULL));
    int arr[10] = {0};
    int *arr_ptr = arr;
    (void)printf("%d\n", *arr_ptr);
    (void)printf("%d\n", (sizeof(arr) / sizeof(*arr)));

    return 0;
};
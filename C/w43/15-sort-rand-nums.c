/**
 * @file 15.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Fill a 10-element int array with random numbers, then print the array to the output.
 * Sort the array ascending(from the smallest to the largest) and then print the sorted array to the output.
 * @version 0.1
 * @date 2025-10-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX 100

// Custom comparator
int comp(const void *a, const void *b)
{
    // If a is smaller, positive value will be returned
    return (*(int *)a - *(int *)b);
}

int main(void)
{
    srand(time(NULL));
    int arr[10];
    for (size_t i = 0; i < 10; i++)
    {
        int num = rand() % (MAX + 1);
        arr[i] = num;
    }

    // Sort the array using qsort
    int n = sizeof(arr) / sizeof(arr[0]);
    qsort(arr, n, sizeof(int), comp);

    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);

    return 0;
}
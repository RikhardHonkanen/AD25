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
#include <stdlib.h>

#define MAX 99

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
    int *arr_ptr = arr;
    for (arr_ptr; (arr_ptr - arr) < sizeof(arr) / sizeof(*arr); arr_ptr++)
    {
        int num = rand() % (MAX + 1);
        *arr_ptr = num;
    }

    // Sort the array using qsort
    int n = sizeof(arr) / sizeof(arr[0]);
    qsort(arr, n, sizeof(int), comp);

    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);

    return 0;
};
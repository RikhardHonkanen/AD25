/**
 * @file 8-sort-arr-pointers.cpp
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Make a program to fill a 10-element int array with random numbers in the range of 0 and 99, then
 * print the array to the output
 * sort the array ascending(from the smallest to the largest) and then
 * Print the sorted array to the output.
 * To fill, print and sort the array use only pointers.
 * @version 0.1
 * @date 2025-12-11
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <iostream>

constexpr int MAX{99};

int comp(const void *a, const void *b)
{
    // If a is smaller, positive value will be returned
    return (*(int *)a - *(int *)b);
}

int main(void)
{
    srand(time(nullptr));
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

    for (int i{0}; i < n; i++)
    {
        std::cout << arr[i] << " " << std::endl;
    }
    return 0;
};
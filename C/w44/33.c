/**
 * @file 33.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief 1) Make a function to fill an array of type int with random numbers.
 * 2) Make a function to print an array of type int to the output.
 * 3) Make a function to print a 2D array of type int to the output.
 * * To handle the arrays use only pointers
 * @version 0.1
 * @date 2025-11-03
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define ARR_LENGTH 10
#define MAX 99

void fill_array(int *array, int length);
void print_array(int *array, int length);

void fill_array(int *array, int length)
{
    for (size_t i = 0; i < length; i++)
    {
        *(array + i) = rand() % (MAX + 1);
    }
};

void print_array(int *array, int length)
{
    for (size_t i = 0; i < length; i++)
    {
        (void)printf("%d\n", *(array + i));
    }
};

int main(void)
{
    srand(time(NULL));
    int arr[ARR_LENGTH] = {0};
    int *arr_ptr = arr;
    fill_array(arr_ptr, ARR_LENGTH);
    print_array(arr_ptr, ARR_LENGTH);

    return 0;
};
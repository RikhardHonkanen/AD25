/**
 * @file 24.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Program to fill a 2D array (2x3) of type int with random numbers
 * in the range of 0 and 99 and then print the array to the terminal.
 * To fill and print the array, use only pointers.
 * @version 0.1
 * @date 2025-10-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX 99

int main(void)
{
    srand(time(NULL));
    int arr[2][3] = {0};
    int *arr_ptr = &arr[0][0];
    int total = sizeof(arr) / sizeof(*arr_ptr);
    int row_size = sizeof(arr[0]) / sizeof(arr[0][0]); // number of columns

    for (int i = 0; i < total; i++) // Populate and print array in same loop
    {
        *(arr_ptr + i) = rand() % (MAX + 1);
        (void)printf("%d ", *(arr_ptr + i));

        if ((i + 1) % row_size == 0) // insert newline at the end of a row
        {
            (void)printf("\n");
        }
    }

    return 0;
};
/**
 * @file exercise-15.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a program to fill a 10-element int array with random numbers, then
 *          1. Print the array to the standard output
 *          2. Sort the array ascending(from the smallest to the largest) and then
 *          3. Print the sorted array to the output.
 *
 *        Example:
 *           The array filled with random numbers: {9, 3, 8, 0, 5, 2, 6, 7, 1, 4}
 *           The sorted array: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }
 *
 * @version 0.1
 * @date 2021-02-23
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ARRAY_LENGTH 10
#define RAND_RANGE_MAX 100

int main(void)
{
    int array[ARRAY_LENGTH] = {0};

    srand(time(0)); // Seed the random function using current time

    // Fill the array with random numbers in the range of 0 and 9 using the rand function
    for (uint8_t i = 0; i < ARRAY_LENGTH; i++)
    {
        array[i] = rand() % RAND_RANGE_MAX;
    }

    // Print the array to the output
    (void)printf("The array filled with random numbers: {");
    for (uint8_t i = 0; i < ARRAY_LENGTH - 1; ++i)
    {
        (void)printf("%d, ", array[i]);
    }
    (void)printf("%d}\n", array[ARRAY_LENGTH - 1]);

    // Sort the array ascending
    for (int i = 0; i < ARRAY_LENGTH; ++i)
    {
        for (int j = i + 1; j < ARRAY_LENGTH; ++j)
        {
            if (array[i] > array[j])
            {
                int temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }

    // Print the sorted array to the output
    (void)printf("The sorted array: {");
    for (uint8_t i = 0; i < ARRAY_LENGTH - 1; ++i)
    {
        (void)printf("%d, ", array[i]);
    }
    (void)printf("%d}\n", array[ARRAY_LENGTH - 1]);

    return 0;
}

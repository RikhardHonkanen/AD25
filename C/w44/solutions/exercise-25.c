/**
 * @file exercise-25.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a function to fill an array of type int with unique random numbers in the range of 1 and 99.
 *        Make another function to print an array of type int to the output.
 *        Example: An array filled with random numbers: {9, 3, 8, 13, 5, 2, 6, 7, 1, 4}
 *
 * @version 0.1
 * @date 2021-03-05
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define RANGE_MIN 1
#define RANGE_MAX 99
#define ARRAY_LENGTH 10U

static void fill_array(size_t len, int arr[len]);
static void print_array(size_t len, const int arr[len]);
static bool in_array(size_t len, const int arr[len], int value);

int main(void)
{
    int array[ARRAY_LENGTH] = {0};

    fill_array(ARRAY_LENGTH, array);

    print_array(ARRAY_LENGTH, array);

    return 0;
}

static bool in_array(size_t len, const int arr[len], int value)
{
    bool found = false;
    for (size_t i = 0; i < len; i++)
    {
        if (arr[i] == value)
        {
            found = true;
            break;
        }
    }

    return found;
}

static void fill_array(size_t len, int arr[len])
{
    srand(time(NULL)); // Seed the random number generator

    arr[0] = RANGE_MIN + rand() % RANGE_MAX;

    for (size_t i = 1; i < len; i++)
    {
        int temp;
        do
        {
            temp = RANGE_MIN + rand() % RANGE_MAX;
        } while (in_array(len, arr, temp));

        arr[i] = temp;
    }
}

static void print_array(size_t len, const int arr[len])
{
    (void)printf("The array filled with random numbers: {");
    for (size_t i = 0; i < len - 1; ++i)
    {
        (void)printf("%d, ", arr[i]);
    }
    (void)printf("%d}\n", arr[len - 1]);
}

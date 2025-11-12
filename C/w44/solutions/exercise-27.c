/**
 * @file exercise-27.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a function to find the largest value in an array of type int and a function to get the average of values in the array.
 *        Make another function to find a specific value in an array of type int. If the value exists in the array,
 *        the index of the value in the array shall be returned; otherwise -1.
 *
 * @version 0.1
 * @date 2021-03-05
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <stdio.h>

#define ARRAY_LENGTH(a) (sizeof(a) / sizeof(a[0]))

static int largest(size_t len, const int *arr);
static float average(size_t len, const int *arr);
static int find(size_t len, const int *arr, int val);

int main(void)
{
    int array[7] = {12, 34, 0, -3, 89, 45, 7};

    (void)printf("The largest value in the array is: %d\n", largest(ARRAY_LENGTH(array), array));

    (void)printf("The average of the values in the array is: %f\n", average(ARRAY_LENGTH(array), array));

    int value = 0;
    int index = find(ARRAY_LENGTH(array), array, value);
    if (index < 0)
    {
        (void)printf("%d does not exist in the array.\n", value);
    }
    else
    {
        (void)printf("Index of the element containing %d is %d.\n", value, index);
    }

    value = 100;
    index = find(value, array, ARRAY_LENGTH(array));
    if (index < 0)
    {
        (void)printf("%d does not exist in the array.\n", value);
    }
    else
    {
        (void)printf("Index of the element containing %d is %d.\n", value, index);
    }

    return 0;
}

static int largest(size_t len, const int *arr)
{
    int result = arr[0];

    for (size_t i = 1; i < len; i++)
    {
        if (result < arr[i])
        {
            result = arr[i];
        }
    }

    return result;
}

static float average(size_t len, const int *arr)
{
    int sum = 0;

    for (size_t i = 0; i < len; i++)
    {
        sum += arr[i];
    }

    return ((float)sum / len);
}

static int find(size_t len, const int *arr, int val)
{
    int result = -1;

    for (size_t i = 0; i < len; i++)
    {
        if (val == arr[i])
        {
            result = i;
            break;
        }
    }

    return result;
}

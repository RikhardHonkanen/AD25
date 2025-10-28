/**
 * @file 27.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Make a function to find the largest value in an array of type int.
 * Make a function to get the average of values in an array of type int.
 * Make a function to find a specific value in an array of type int.
 * If the value exists in the array, index of the element containing the value in the array shall be returned; otherwise -1.
 * @version 0.1
 * @date 2025-10-28
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX 99
#define SIZE 10

void fill_array(int *array, int length);
int largest_value(int *array, int length);
void print_array(int *array, int length);
float average_value(int *array, int length);
int find_value(int *array, int length, int target);

void fill_array(int *array, int length)
{
    for (int i = 0; i < length; i++)
    {
        *(array + i) = rand() % (MAX + 1);
    }
};

void print_array(int *array, int length)
{
    (void)printf("Array:\n");
    for (int i = 0; i < length; i++)
    {
        (void)printf("%d\n", *(array + i));
    }
};

int largest_value(int *array, int length)
{
    int max = -1;
    for (int i = 0; i < length; i++)
    {
        if (*(array + i) > max)
        {
            max = *(array + i);
        }
    }
    return max;
};

float average_value(int *array, int length)
{
    int total = 0;
    for (int i = 0; i < length; i++)
    {
        total += *(array + i);
    }
    return (float)total / length;
};

int find_value(int *array, int length, int target)
{
    int index = -1;
    for (int i = 0; i < length; i++)
    {
        if (*(array + i) == target)
        {
            index = i;
        }
    }
    return index;
};

int main(void)
{
    srand(time(NULL));
    int random_numbers[SIZE] = {0};
    int *random_nums_ptr = random_numbers;
    int target = rand() % (MAX + 1);
    fill_array(random_nums_ptr, SIZE);
    print_array(random_nums_ptr, SIZE);
    (void)printf("Largest number: %d\n", largest_value(random_nums_ptr, SIZE));
    (void)printf("Average value: %f\n", average_value(random_nums_ptr, SIZE));
    int target_index = find_value(random_nums_ptr, SIZE, target);

    if (target_index == -1)
    {
        (void)printf("Target value %d not found\n", target);
    }
    else
    {
        (void)printf("Target value %d found at index %d\n", target, target_index);
    }

    return 0;
};
/**
 * @file 25.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Make a function to fill an array of type int with unique random numbers in the range of 1 and 99.
 * Make another function to print an array of type int to the output.
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
    int random_numbers[10] = {0};
    int *random_nums_ptr = random_numbers;
    fill_array(random_nums_ptr, 10);
    print_array(random_nums_ptr, 10);

    return 0;
};
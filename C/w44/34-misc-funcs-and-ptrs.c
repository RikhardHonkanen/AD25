/**
 * @file 36.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief 1) Make a function to fill an array of type int with random numbers in the range 0 and 99.
 * 2) Make a function to print an array of type int to the output.
 * 3) Make a function to swap values of two int variables.
 *
 * 4) Make a function to sort an array of type int. The function shall accept a function pointer of type compare_t to compare two values
 *     typedef bool (*compare_t)(int, int);
 * void sort_array(int *arr, size_t length, compare_t compare);
 *     a) When you sort the array, the compare function shall be used to compare the elements
 *     b) To swap the elements, the swap function of step 3 shall be used.
 *     c) Implement two functions, compare_asc and compare_desc, to sort the array ascending and descending
 * bool compare_asc(int a, int b); and bool compare_desc(int a, int b);
 *     d) You shall be able to sort the array ascending and descending by calling sort_array with different compare functions
 * @version 0.1
 * @date 2025-11-02
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 99
#define LENGTH 10

typedef bool (*compare_t)(int, int);

void fill_array(int *array, int length);
void print_array(int *array, int length);
void swap_values(int *a, int *b);
bool compare_asc(int a, int b);
bool compare_desc(int a, int b);
void sort_array(int *arr, size_t length, compare_t compare);

void fill_array(int *array, int length)
{
    for (int i = 0; i < length; i++)
    {
        *(array + i) = rand() % (MAX + 1);
    }
};

void print_array(int *array, int length)
{
    for (int i = 0; i < length; i++)
    {
        (void)printf("%d\n", *(array + i));
    }
};

void swap_values(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
};
bool compare_asc(int a, int b)
{
    return (a < b);
};
bool compare_desc(int a, int b)
{
    return (b < a);
};
void sort_array(int *arr, size_t length, compare_t compare) // Selection sort
{
    for (size_t i = 0; i < length - 1; i++)
    {
        size_t best_idx = i;
        for (size_t j = i + 1; j < length; j++)
        {
            if (compare(arr[j], arr[best_idx]))
            {
                best_idx = j;
            }
        }
        swap_values(&arr[i], &arr[best_idx]);
    }
};

int main(void)
{
    srand(time(NULL));
    int random_numbers[LENGTH] = {0};
    int *random_nums_ptr = random_numbers;
    fill_array(random_nums_ptr, LENGTH);
    sort_array(random_nums_ptr, LENGTH, compare_asc);
    (void)printf("Ascending sorted:\n");
    print_array(random_nums_ptr, LENGTH);
    sort_array(random_nums_ptr, LENGTH, compare_desc);
    (void)printf("Descending sorted:\n");
    print_array(random_nums_ptr, LENGTH);

    int a = 1, b = 8;
    swap_values(&a, &b);
    printf("a is %d, b is %d\n", a, b);

    return 0;
};
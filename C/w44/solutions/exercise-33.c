/**
 * @file exercise-33.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief 1) Make a function to fill an array of type int with random numbers.
 *        2) Make a function to print an array of type int to the output.
 *        3) Make a function to print a 2D array of type int to the output.
 *
 *         * To handle the arrays use only pointers
 *
 * @version 0.1
 * @date 2021-03-06
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define ROWS 2U
#define COLUMNS 3U
#define ARRAY_LEN 10U

static void fill_array(size_t len, int *arr);
static void print_array(size_t len, const int *arr);
static void print_2d_array(size_t rows, size_t columns, const int (*arr)[]);

int main(void)
{
    int array[ARRAY_LEN] = {0};

    fill_array(ARRAY_LEN, array);

    (void)printf("Array: ");
    print_array(ARRAY_LEN, array);

    int matrix[ROWS][COLUMNS] = {{1, 2, 3}, {4, 5, 6}};

    print_2d_array(ROWS, COLUMNS, (const int (*)[])matrix);

    return 0;
}

static void print_2d_array(size_t rows, size_t columns, const int (*arr)[])
{
    int (*matrix)[columns] = (int (*)[columns])arr;

    for (int (*rptr)[columns] = matrix; (rptr - matrix) < rows; rptr++)
    {
        for (int *cptr = *rptr; (cptr - *rptr) < columns; cptr++)
        {
            (void)printf("%d\t", *cptr);
        }
        (void)printf("\n");
    }
}

static void print_array(size_t len, const int *arr)
{
    int *iptr = (int *)arr;

    (void)printf("{");
    while ((iptr - arr) < (len - 1))
    {
        (void)printf("%d, ", *iptr++);
    }
    (void)printf("%d}\n", *iptr);
}

static void fill_array(size_t len, int *arr)
{
    srand(time(NULL));

    for (int *iptr = arr; (iptr - arr) < len; iptr++)
    {
        *iptr = rand();
    }
}

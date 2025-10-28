#include <stdio.h>
#include <stddef.h>

// gcc -Wall -Wextra -Wpedantic -Werror func-arr.c -o main && ./main && rm main

static void print0(int length, const int arr[])
{
    for (int i = 0; i < length; i++)
    {
        (void)printf("array[%d] = %d\n", i, arr[i]);
    }
}

static void print1(int length, const int arr[length])
{
    for (int i = 0; i < length; i++)
    {
        (void)printf("array[%d] = %d\n", i, arr[i]);
    }
}

static void print2(int length, const int *arr)
{
    for (int i = 0; i < length; i++)
    {
        (void)printf("array[%d] = %d\n", i, arr[i]);
    }
}

static void print3(size_t m, size_t n, const int matrix[m][n])
{
    for (size_t i = 0; i < m; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            (void)printf("%d\t", matrix[i][j]);
        }
        (void)printf("\n");
    }
}

static void print4(size_t m, size_t n, const int (*matrix)[n])
{
    for (size_t i = 0; i < m; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            (void)printf("%d\t", matrix[i][j]);
        }
        (void)printf("\n");
    }
}

// Not the best practice
static void print5(size_t m, size_t n, const int *matrix)
{
    const int(*ptr)[n] = (const int(*)[n])matrix;

    for (size_t i = 0; i < m; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            (void)printf("%d\t", ptr[i][j]);
        }
        (void)printf("\n");
    }
}

// Not the best practice
static void print6(size_t m, size_t n, const int **matrix)
{
    const int(*ptr)[n] = (const int(*)[n])matrix;

    for (size_t i = 0; i < m; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            (void)printf("%d\t", ptr[i][j]);
        }
        (void)printf("\n");
    }
}

int main(void)
{
    int array[6] = {1, 2, 3, 4, 5, 6};

    print0(sizeof(array) / sizeof(*array), array);
    (void)printf("\n");

    print1(sizeof(array) / sizeof(*array), array);
    (void)printf("\n");

    print2(sizeof(array) / sizeof(*array), array);
    (void)printf("\n");

    int matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};

    print3(2, 3, (const int(*)[3])matrix);
    (void)printf("\n");

    print4(2, 3, (const int(*)[3])matrix);
    (void)printf("\n");

    print5(2, 3, (const int *)matrix); // Not the best practice
    (void)printf("\n");

    (void)printf("\n");
    print6(2, 3, (const int **)matrix); // Not the best practice

    return 0;
}

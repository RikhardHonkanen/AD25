/**
 * @file arr-ptr.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of array & pointers
 * @version 0.1
 * @date 2022-10-28
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>

#define M 2
#define N 3

typedef int array_t[N]; // array_t is an array type

int main(void)
{
    // A single dimension array declared using the subscript operator
    int arr1[N] = {1, 2, 3}; // // It is equivalent to: array_t arr1 = {1, 2, 3};

    // Getting access to the elements using an index and the subscript operator
    (void)printf("Array 1: {");
    for (int i = 0; i < N - 1; i++)
    {
        (void)printf("%d, ", arr1[i]);
    }
    (void)printf("%d}\n", arr1[N - 1]);

    // A single dimension array declared using a pointer
    int *const arr2 = (int[N]){1, 2, 3};

    // Getting access to the elements using a pointer.
    (void)printf("Array 2: {");
    for (int *ptr = arr2; (ptr - arr2) < N - 1; ptr++)
    {
        (void)printf("%d, ", *ptr);
    }
    (void)printf("%d}\n", *(arr2 + N - 1));

    // A single dimension array declared using the array type
    array_t arr3 = {1, 2, 3}; // It is equivalent to: int arr3[N] = {1, 2, 3};

    // Getting access to the elements using a pointer.
    (void)printf("Array 3: {");
    for (int *ptr = arr3; (ptr - arr3) < N - 1; ptr++)
    {
        (void)printf("%d, ", *ptr);
    }
    (void)printf("%d}\n", *(arr3 + N - 1));

    // A 2D array declared using the subscript operator
    int mtx1[M][N] = {{1, 2, 3}, {4, 5, 6}};

    // Getting access to the elements using indexes and the subscript operator
    (void)printf("Matrix 1: {");
    for (int i = 0; i < sizeof(mtx1) / sizeof(mtx1[0]); i++)
    {
        (void)printf("{");
        for (int j = 0; j < sizeof(mtx1[0]) / sizeof(mtx1[0][0]) - 1; j++)
        {
            (void)printf("%d, ", mtx1[i][j]);
        }

        if (i == (sizeof(mtx1) / sizeof(mtx1[0]) - 1))
        {
            (void)printf("%d}", mtx1[i][sizeof(mtx1[0]) / sizeof(mtx1[0][0]) - 1]);
        }
        else
        {
            (void)printf("%d}, ", mtx1[i][sizeof(mtx1[0]) / sizeof(mtx1[0][0]) - 1]);
        }
    }
    (void)printf("}\n");

    // A 2D array declared using a pointer
    int(*const mtx2)[N] = (int[M][N]){{1, 2, 3}, {4, 5, 6}};

    // Getting access to the elements using pointers
    (void)printf("Matrix 2: {");
    for (int(*iptr)[N] = mtx2; (iptr - mtx2) < M; iptr++)
    {
        (void)printf("{");
        for (int *jptr = *iptr; (jptr - *iptr) < N - 1; jptr++)
        {
            (void)printf("%d, ", *jptr);
        }

        if (iptr - mtx2 == M - 1)
        {
            (void)printf("%d}", *(*iptr + N - 1));
        }
        else
        {
            (void)printf("%d}, ", *(*iptr + N - 1));
        }
    }
    (void)printf("}\n");

    // A 2D array declared using the array type
    array_t mtx3[M] = {{1, 2, 3}, {4, 5, 6}}; // It is equivalent to: int mtx3[M][N] = {{1, 2, 3}, {4, 5, 6}};

    // Getting access to the elements using pointers
    (void)printf("Matrix 3: {");
    for (array_t *iptr = mtx3; (iptr - mtx3) < M; iptr++)
    {
        (void)printf("{");
        for (int *jptr = *iptr; (jptr - *iptr) < N - 1; jptr++)
        {
            (void)printf("%d, ", *jptr);
        }

        if (iptr - mtx3 == M - 1)
        {
            (void)printf("%d}", *(*iptr + N - 1));
        }
        else
        {
            (void)printf("%d}, ", *(*iptr + N - 1));
        }
    }
    (void)printf("}\n");

    return 0;
}
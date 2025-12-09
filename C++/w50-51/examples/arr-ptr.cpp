/**
 * @file arr-ptr.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of array & pointers
 * @version 0.1
 * @date 2022-10-28
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>

constexpr int M = 2;
constexpr int N = 3;

using array_t = int[N]; // array_t is an array type

int main(void)
{
    // A single dimension array declared using the subscript operator
    int arr1[N]{1, 2, 3}; // // It is equivalent to: array_t arr1{1, 2, 3};

    // Getting access to the elements using an index and the subscript operator
    std::cout << "Array 1: {";
    for (int i = 0; i < N - 1; i++)
    {
        std::cout << arr1[i] << ", ";
    }
    std::cout << arr1[N - 1] << "}" << std::endl;

    // A single dimension array declared using the array type
    array_t arr2 = {1, 2, 3}; // It is equivalent to: int arr2[N]{1, 2, 3};

    // Getting access to the elements using a pointer.
    std::cout << "Array 2: {";
    for (int *ptr = arr2; (ptr - arr2) < N - 1; ptr++)
    {
        std::cout << *ptr << ", ";
    }
    std::cout << *(arr2 + N - 1) << "}" << std::endl;

    // A 2D array declared using the subscript operator
    int mtx1[M][N]{{1, 2, 3}, {4, 5, 6}};

    // Getting access to the elements using indexes and the subscript operator
    std::cout << "Matrix 1: {";
    for (size_t i = 0; i < sizeof(mtx1) / sizeof(mtx1[0]); i++)
    {
        std::cout << "{";
        for (size_t j = 0; j < sizeof(mtx1[0]) / sizeof(mtx1[0][0]) - 1; j++)
        {
            std::cout << mtx1[i][j] << ", ";
        }

        if (i == (sizeof(mtx1) / sizeof(mtx1[0]) - 1))
        {
            std::cout << mtx1[i][sizeof(mtx1[0]) / sizeof(mtx1[0][0]) - 1] << "}";
        }
        else
        {
            std::cout << mtx1[i][sizeof(mtx1[0]) / sizeof(mtx1[0][0]) - 1] << "}, ";
        }
    }
    std::cout << "}" << std::endl;

    // A 2D array declared using the array type
    array_t mtx2[M] = {{1, 2, 3}, {4, 5, 6}}; // It is equivalent to: int mtx2[M][N]{{1, 2, 3}, {4, 5, 6}};

    // Getting access to the elements using pointers
    std::cout << "Matrix 2: {";
    for (array_t *iptr = mtx2; (iptr - mtx2) < M; iptr++)
    {
        std::cout << "{";
        for (int *jptr = *iptr; (jptr - *iptr) < N - 1; jptr++)
        {
            std::cout << *jptr << ", ";
        }

        if (iptr - mtx2 == M - 1)
        {
            std::cout << *(*iptr + N - 1) << "}";
        }
        else
        {
            std::cout << *(*iptr + N - 1) << "}, ";
        }
    }
    std::cout << "}" << std::endl;

    return 0;
}
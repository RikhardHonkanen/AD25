/**
 * @file exercise-12.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief 1) Make a function to fill an array of type int with random numbers int the range 0 and 99.
 *        2) Make a function to print an array of type int to the output.
 *        3) Make a function to swap values of two int variables.
 *        4) Make a function to sort an array of type int.
 *           The function shall accept a function pointer of type compare_t to compare two values
 *                        using compare_t = bool (*)(int, int);
 *                void sort_array(int *arr, size_t length, compare_t compare);
 *
 *           a) When you sort the array, the compare function shall be used to compare the elements
 *           b) To swap the elements, the swap function shall be used.
 *           c) Implement two functions, compare_asc and compare_desc, to sort the array ascending and descending
 *                       bool compare_asc(int a, int b); and bool compare_desc(int a, int b);
 *
 *           d) You shall be able to sort the array ascending and descending by calling sort_array with different compare functions
 *
 *        * To handle the arrays use only pointers
 *
 * @version 0.1
 * @date 2021-03-06
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <ctime>
#include <cstdlib>
#include <iostream>

using compare_t = bool (*)(int, int);

static void fill_array(int len, int *arr);
static void print_array(int len, const int *arr);
static void sort_array(int len, int *arr, compare_t compare);

static bool compare_asc(int a, int b) { return (a > b); }
static bool compare_desc(int a, int b) { return (a < b); }

constexpr int RANGE_MAX = 99;
constexpr int ARRAY_LEN = 10;

int main(void)
{
    int array[ARRAY_LEN];

    fill_array(ARRAY_LEN, array);

    std::cout << "The array: ";
    print_array(ARRAY_LEN, array);

    sort_array(ARRAY_LEN, array, compare_asc);

    std::cout << "The array sorted ascending: ";
    print_array(ARRAY_LEN, array);

    sort_array(ARRAY_LEN, array, compare_desc);

    std::cout << "The array sorted descending: ";
    print_array(ARRAY_LEN, array);

    return 0;
}

static void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void sort_array(int len, int *arr, compare_t compare)
{
    for (int *iptr = arr; (iptr - arr) < len; iptr++)
    {
        for (int *jptr = iptr + 1; (jptr - arr) < len; jptr++)
        {
            if (compare(*iptr, *jptr))
            {
                swap(iptr, jptr);
            }
        }
    }
}

static void print_array(int len, const int *arr)
{
    int *iptr = const_cast<int *>(arr);

    std::cout << "{";
    while ((iptr - arr) < (len - 1))
    {
        std::cout << *iptr++ << ", ";
    }
    std::cout << *iptr << "}" << std::endl;
}

static void fill_array(int len, int *arr)
{
    std::srand(std::time(nullptr));

    for (int *iptr = arr; (iptr - arr) < len; iptr++)
    {
        *iptr = std::rand() % (RANGE_MAX + 1);
    }
}

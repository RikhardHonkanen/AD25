/**
 * @file 12.cpp
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief 1) Make a function to fill an array of type int with random numbers int the range 0 and 99.
 * 2) Make a function to print an array of type int to the output.
 * 3) Make a function to swap values of two int variables.
 * 4) Make a function to sort an array of type int. The function shall accept a function pointer of type compare_t to compare two values

    using compare_t = bool (*)(int, int);
    void sort_array(size_t length, int *arr, compare_t compare);

    a) When you sort the array, the compare function shall be used to compare the elements

    b) To swap the elements, the swap function of step 3 shall be used.

    c) Implement two functions, compare_asc and compare_desc, to sort the array ascending and descending

    bool compare_asc(int a, int b); and bool compare_desc(int a, int b);

    d) You shall be able to sort the array ascending and descending by calling sort_array with different compare functions

* To handle the arrays use only pointers
 * @version 0.1
 * @date 2025-12-16
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <iostream>
#include <array>

constexpr int ARR_LENGTH{10};
constexpr int MAX_NUM{100};

void fill_array(size_t length, int *arr);
void print_array(size_t length, int *arr);
void sort_array(size_t length, int *arr, compare_t compare);
void swap_values(int &a, int &b);
bool compare_asc(int a, int b);
bool compare_desc(int a, int b);

using compare_t = bool (*)(int, int);

void fill_array(size_t length, int *arr)
{
    for (size_t i{0}; i < length; i++)
    {
        *(arr + i) = rand() % MAX_NUM;
    }
}
void print_array(size_t length, int *arr)
{
    for (size_t i{0}; i < length; i++)
    {
        std::cout << *(arr + i) << '\t';
    }
    std::cout << '\n';
}
void sort_array(size_t length, int *arr, compare_t compare)
{
    for (int i{0}; i < length; ++i)
    {
        for (int j{i + 1}; j < length; ++j)
        {
            if (compare(*(arr + i), *(arr + j)))
            {
                swap_values(*(arr + i), *(arr + j));
            }
        }
    }
}
void swap_values(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}
bool compare_asc(int a, int b)
{
    return a > b;
}
bool compare_desc(int a, int b)
{
    return a < b;
}

int main(void)
{
    srand(time(nullptr));

    std::array<int, ARR_LENGTH> nums{};
    fill_array(ARR_LENGTH, nums.data());
    std::cout << "Array: \n";
    print_array(ARR_LENGTH, nums.data());

    sort_array(ARR_LENGTH, nums.data(), compare_asc);
    std::cout << "Array ascending: \n";
    print_array(ARR_LENGTH, nums.data());

    sort_array(ARR_LENGTH, nums.data(), compare_desc);
    std::cout << "Array descending: \n";
    print_array(ARR_LENGTH, nums.data());

    return 0;
};
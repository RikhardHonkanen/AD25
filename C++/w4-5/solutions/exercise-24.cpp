/**
 * @file exercise-1.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief 1) Make a template print function to print an array.
 *        2) Make a template function pointer type to point to the print function
 *        3) Call the print function using an instance of the function pointer type
 *           to print an example array of type int.
 *
 * @version 0.1
 * @date 2021-06-11
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <iostream>

template <typename T, size_t SIZE>
static void print(const T arr[SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        std::cout << arr[i] << "\t";
    }

    std::cout << std::endl;
}

template <typename T, size_t SIZE>
using func_t = void (*)(const T[SIZE]);

template <typename T>
struct array
{
};

template <typename T, size_t SIZE>
struct array<T[SIZE]>
{
    using elem_type = T;
    static constexpr size_t size{SIZE};
};

int main(void)
{
    int arr[]{1, 2, 3, 4, 5};

    using type = array<decltype(arr)>::elem_type;
    constexpr size_t size{array<decltype(arr)>::size};

    func_t<type, size> pptr{print<type, size>};

    pptr(arr);

    return 0;
}

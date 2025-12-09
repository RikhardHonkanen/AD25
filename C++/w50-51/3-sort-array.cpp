/**
 * @file 3-sort-array.cpp
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Make a program to fill a 10 element int array with positive random numbers lessr than 100, then
 * Print the array to the output.
 * Sort the array ascending(from the smallest to the largest) and then
 * Print the sorted array to the output.
 * @version 0.1
 * @date 2025-12-09
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <iostream>
#include <array>
#include <functional>

int main(void)
{
    srand(time(nullptr));
    std::array<int, 10> arr{0};

    for (int i{0}; i < 10; i++)
    {
        arr[i] = rand() % 100;
    }
    std::cout << "Random numbers:" << std::endl;
    for (int i{0}; i < 10; i++)
    {
        std::cout << arr[i] << std::endl;
    }

    std::sort(arr.begin(), arr.end());
    std::cout << "Sorted random numbers:" << std::endl;
    for (int i{0}; i < 10; i++)
    {
        std::cout << arr[i] << std::endl;
    }

    return 0;
};
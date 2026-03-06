/**
 * @file exercise-3.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a program to fill a 10 elements int array with random numbers, then
 *        1) Print the array to the output.
 *        2) Sort the array ascending(from the smallest to the largest) and then
 *        3) Print the sorted array to the output.
 *
 *        Example:
 *           The array filled with random numbers: {9, 3, 8, 0, 5, 2, 6, 7, 1, 4}
 *           The sorted array: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }
 *
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <ctime>
#include <cstdlib>
#include <iostream>

constexpr int LEN{10};
constexpr int RANGE_MAX{100};

int main(void)
{
    int array[LEN];

    std::srand(time(nullptr));

    for (int i = 0; i < LEN; i++)
    {
        array[i] = std::rand() % RANGE_MAX;
    }

    std::cout << "The filled array: {";
    for (int i = 0; i < LEN - 1; i++)
    {
        std::cout << array[i] << ", ";
    }
    std::cout << array[LEN - 1] << "}" << std::endl;

    for (int i = 0; i < LEN; i++)
    {
        for (int j = i + 1; j < LEN; j++)
        {
            if (array[i] > array[j])
            {
                int temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }

    std::cout << "The sorted array: {";
    for (int i = 0; i < LEN - 1; i++)
    {
        std::cout << array[i] << ", ";
    }
    std::cout << array[LEN - 1] << "}" << std::endl;

    return 0;
}

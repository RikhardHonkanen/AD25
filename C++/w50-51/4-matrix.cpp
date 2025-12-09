/**
 * @file 4-matrix.cpp
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Write a program to make a 2 dimensional(3x3) array of type int.
 * Then ask the user to enter all the elements and then print the array to the output.
 * Print the array to the output like a table.

 * @version 0.1
 * @date 2025-12-09
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <iostream>
#include <array>

int main(void)
{
    srand(time(nullptr));
    std::array<std::array<int, 3>, 3> m = {};

    for (int i{0}; i < 3; i++)
    {
        std::cout << std::endl;
        for (int j{0}; j < 3; j++)
        {
            int num;
            std::cout << "Enter random number " << (3 * i + j + 1) << ": ";
            std::cin >> num;
            m[i][j] = num;
        }
    }
    for (int i{0}; i < 3; i++)
    {
        std::cout << std::endl;
        for (int j{0}; j < 3; j++)
        {
            std::cout << m[i][j] << " ";
        }
    }
    std::cout << std::endl;

    return 0;
};
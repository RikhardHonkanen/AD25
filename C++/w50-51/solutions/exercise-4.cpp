/**
 * @file exercise-4.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Write a program to make a 2 dimensional(3x3) array of type int.
 *        Then ask the user to enter all the elements and then print the array to the output.
 *        Print the array to the output like a table. For example:
 *
 *        Example:
 *           123   234  456
 *           54    678  987
 *           12    987  100
 *
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>

constexpr int ROWS = 3;
constexpr int COLUMNS = 3;

int main(void)
{
    int matrix[ROWS][COLUMNS];

    for (int row = 0; row < ROWS; ++row)
    {
        for (int column = 0; column < COLUMNS; ++column)
        {
            std::cout << "Enter the value of matrix[" << row << "][" << column << "]: ";
            std::cin >> matrix[row][column];
        }
    }

    for (const auto &row : matrix)
    {
        for (const auto elem : row)
        {
            std::cout << elem << "\t";
        }
        std::cout << std::endl;
    }

    return 0;
}

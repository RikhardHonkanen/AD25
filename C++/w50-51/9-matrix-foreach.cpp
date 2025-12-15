/**
 * @file 9-matrix-pointers.cpp
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Make a program to fill a 2D array(2x3) of type int with random numbers
 * in the range of 0 and 99 and then print the array to the terminal.
 * To fill and print the array, use pointers.
 * @version 0.1
 * @date 2025-12-11
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <iostream>
#include <array>

constexpr int ROWS{2};
constexpr int COLUMNS{3};
constexpr int MAX{100};

int main(void)
{
    srand(time(nullptr));
    std::array<std::array<int, COLUMNS>, ROWS> matrix{0};

    for (auto &row : matrix)
    {
        for (auto &col : row)
        {
            col = rand() % MAX;
        }
    }

    int *ptr = &matrix[0][0];
    for (size_t i = 0; i < ROWS * COLUMNS; ++i)
    {
        if (i % COLUMNS == 0)
        {
            std::cout << std::endl;
        }

        std::cout << *(ptr + i) << '\t';
    }

    return 0;
};
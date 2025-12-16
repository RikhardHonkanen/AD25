/**
 * @file exercise-9.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a program to fill a 2D array (2x3) of type int with random numbers
 *        in the range of 0 and 99 and then print the array to the terminal.
 *
 *        To fill and print the array, use only pointers.
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

constexpr int RANGE_MAX = 99;
constexpr int COLUMNS = 3;
constexpr int ROWS = 2;

int main(void)
{
    int matrix[ROWS][COLUMNS];

    std::srand(std::time(nullptr));

    // Fill the array using random numbers
    for (int (*rptr)[COLUMNS] = matrix; (rptr - matrix) < ROWS; rptr++)
    {
        for (int *cptr = *rptr; (cptr - *rptr) < COLUMNS; cptr++)
        {
            *cptr = std::rand() % (RANGE_MAX + 1);
        }
    }

    // Print the array to the terminal
    for (int (*rptr)[COLUMNS] = matrix; (rptr - matrix) < ROWS; rptr++)
    {
        for (int *cptr = *rptr; (cptr - *rptr) < COLUMNS; cptr++)
        {
            std::cout << *cptr << "\t";
        }
        std::cout << std::endl;
    }

    return 0;
}
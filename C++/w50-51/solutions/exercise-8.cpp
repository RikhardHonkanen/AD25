/**
 * @file exercise-8.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a program to fill a 10-element int array with random numbers
 *        in the range of 0 and 99, then
 *          1) Print the array to the output.
 *          2) Sort the array ascending(from the smallest to the largest) and then
 *          3) Print the sorted array to the output.
 *
 *          To fill, print and sort the array use only pointers.
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
constexpr int ARRAY_LENGTH = 10;

int main(void)
{
    int *aptr = nullptr;
    int array[ARRAY_LENGTH];

    std::srand(std::time(nullptr)); // Seed the random number generator

    // Fill the array using random numers
    for (aptr = array; (aptr - array) < ARRAY_LENGTH; aptr++)
    {
        *aptr = std::rand() % (RANGE_MAX + 1); // Generate a random number
    }

    // Print the array to the terminal
    std::cout << "The array filled with random numbers: {";
    for (aptr = array; (aptr - array) < ARRAY_LENGTH - 1; aptr++)
    {
        std::cout << *aptr << ", ";
    }
    std::cout << *aptr << "}" << std::endl;

    // Sort the array
    for (int *iptr = array; (iptr - array) < ARRAY_LENGTH; iptr++)
    {
        for (int *jptr = iptr + 1; (jptr - array) < ARRAY_LENGTH; jptr++)
        {
            if (*iptr > *jptr)
            {
                int temp = *iptr;
                *iptr = *jptr;
                *jptr = temp;
            }
        }
    }

    // Print the array to the terminal
    std::cout << "The sorted array: {";
    for (aptr = array; (aptr - array) < ARRAY_LENGTH - 1; aptr++)
    {
        std::cout << *aptr << ", ";
    }
    std::cout << *aptr << "}" << std::endl;

    return 0;
}
/**
 * @file exercise-6.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a program to read a number in the range of 0 and 255 from the standard input and
 *        then print the binary form of the number to the output.
 *        While the enetered number is not in the range, the program shall ask for a new number.
 *        Use the bitwise operators!
 *
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <ios>
#include <limits>
#include <iostream>

constexpr int BYTE_BITS = 8;
constexpr int RANGE_MIN = 0;
constexpr int RANGE_MAX = 255;

int main(void)
{
    int number;

    do
    {
        std::cout << "Enter a number in the range of " << RANGE_MIN << " and " << RANGE_MAX << ": ";
        std::cin >> number;
        if (std::cin.fail())
        {
            number = -1;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

    } while ((number < RANGE_MIN) || (number > RANGE_MAX));

    std::cout << number << " in the binary format: ";

    int i = BYTE_BITS * sizeof(number) - 1;

    for (; i >= 0; i--)
    {
        if ((number & (1 << i)) != 0)
        {
            break;
        }
    }

    if (i < 0)
    {
        std::cout << "0";
    }
    else
    {
        for (; i >= 0; i--)
        {
            std::cout << (((number & (1 << i)) == 0) ? "0" : "1");
        }
    }

    std::cout << std::endl;

    return 0;
}
/**
 * @file exercise-2.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief 1. Read a lowercase letter from terminal and convert it to uppercase and then print the converted letter to the terminal.
 *        2. Generate two random numbers and print them to the terminal.
 *
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <ctime>
#include <iostream>

int main(void)
{
    char letter;

    std::cout << "Enter a lowercase letter(a-z): ";
    std::cin >> letter;

    std::cout << "The converted letter: ";
    std::cout << static_cast<char>(letter - ('a' - 'A')) << std::endl;

    std::srand(std::time(nullptr));
    std::cout << "Random Numbers: " << std::rand() << ", " << std::rand() << std::endl;

    return 0;
}

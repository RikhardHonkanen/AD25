/**
 * @file 2-lower-upper-randnum
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief 1. Read a lowercase letter from terminal and convert it to uppercase and then print the converted letter to the terminal.
 * 2. Generate two random numbers and print them to the terminal.
 * @version 0.1
 * @date 2025-12-09
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <iostream>

int main(void)
{
    srand(time(nullptr));
    char c;

    std::cout << "Enter a lowercase letter: ";
    std::cin >> c;
    char upper = toupper(c);
    std::cout << "Uppercase: " << upper << std::endl;

    for (int i{0}; i < 2; i++)
    {
        int randnum = rand() % 100;
        std::cout << "Random " << (i + 1) << ": " << randnum << std::endl;
    }

    return 0;
};
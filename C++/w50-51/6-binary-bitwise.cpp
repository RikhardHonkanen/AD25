/**
 * @file 6-binary-bitwise
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * Make a program to read a number in the range of 0 and 255 from the standard input
 * and then print the binary form of the number to the output. While the enetered number is not in the range,
 * the program shall ask for a new number. Use the bitwise operators!
 * @brief
 * @version 0.1
 * @date 2025-12-11
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <iostream>
#include <cstdint>

int main(void)
{
    int temp;
    uint8_t num;

    std::cout << "Enter a number between 0 and 255: ";
    do
    {
        std::cin >> temp;
        num = static_cast<uint8_t>(temp);
        std::cout << std::endl;
    } while (num < 0 || num > 255);

    std::cout << "Binary:" << std::endl;
    for (int i{sizeof(num) * 8 - 1}; i >= 0; i--)
    {
        std::cout << ((num >> i) & 1);
    }
    std::cout << std::endl;

    return 0;
};
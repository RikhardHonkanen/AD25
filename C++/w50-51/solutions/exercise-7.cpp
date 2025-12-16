/**
 * @file exercise-7.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a program to get a string from the standard input,
 *        then print the string to the output in the reversed order.
 *        Don’t use a standard function. Implement the algorithm by yourself.
 *
 *        Example:
 *           Enter a string: ABCDEF
 *           Reversed string: FEDCBA
 *
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <string>
#include <iostream>

int main(void)
{
    std::string str;

    std::cout << "Enter a string: ";
    std::cin >> str;

    // Reverse the string
    int left = 0, right = str.length() - 1;
    while (left < right)
    {
        char temp = str[left];
        str[left] = str[right];
        str[right] = temp;

        right--;
        left++;
    }

    std::cout << "Reversed string: " << str << std::endl;

    return 0;
}
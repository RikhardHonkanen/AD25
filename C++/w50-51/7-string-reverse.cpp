/**
 * @file 7-string-reverse.cpp
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Make a program to get a string from the standard input, then print the string to the output in the reversed order.
 * Don’t use a standard function. Implement the algorithm by yourself.
 * For example:
 *  Enter a string: ABCDEF
 *  Reversed string: FEDCBA
 * @version 0.1
 * @date 2025-12-11
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <iostream>
#include <string>
#include <cstdint>

int main(void)
{
    std::string s{};
    std::cout << "Enter a string: ";
    std::cin >> s;
    std::cout << std::endl;

    uint64_t n{s.length()};
    int l = 0, r = n - 1;
    while (l < r)
    {
        char temp = s[l];
        s[l] = s[r];
        s[r] = temp;

        l++;
        r--;
    }

    std::cout << s << std::endl;

    return 0;
};
/**
 * @file 11-string-cases
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Make a program to read a std::string from a terminal and then convert
 * the uppercase letters to lowercase and vice versa and
 * in the end print the string to the standard output.
 * To change and print the string use only pointers.
 * @version 0.1
 * @date 2025-12-16
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <iostream>
#include <string>
#include <cctype>

int main(void)
{
    std::string user_input;
    std::cout << "Enter a string: \n";
    std::getline(std::cin, user_input);

    char *sptr = user_input.data();

    for (size_t i{0}; i < user_input.length(); i++)
    {
        if (islower(*(sptr + i)))
        {
            *(sptr + i) = toupper(*(sptr + i));
        }
        else
        {
            *(sptr + i) = tolower(*(sptr + i));
        }
    }

    std::cout << user_input << std::endl;

    return 0;
};
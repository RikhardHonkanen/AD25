/**
 * @file exercise-11.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a program to read a std::string from a terminal and then convert
 *        the uppercase letters to lowercase and vice versa and
 *        in the end print the string to the standard output.
 *
 *        To change and print the string use only pointers.
 *
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <cctype>
#include <string>
#include <iostream>

int main(void)
{
    std::string str;

    std::cout << "The input string: ";
    std::getline(std::cin, str);

    // Change cases of the letters
    for (char *cptr = const_cast<char *>(str.c_str()); *cptr != '\0'; cptr++)
    {
        *cptr = std::isupper(*cptr) ? std::tolower(*cptr) : std::toupper(*cptr);
    }

    std::cout << "The converted string: " << str << std::endl;

    return 0;
}
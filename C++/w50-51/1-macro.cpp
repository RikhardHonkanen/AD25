/**
 * @file 1-macro.cpp
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Make a program to print a macro named NUMBER whose value is an integer to the terminal.
 * If NUMBER has not been defined, an error message shall be generated and compilation shall be terminated.
 * If NUMBER is not equal to 8 or 16,  an error message shall be generated and compilation shall be terminated.
 * The macro shall be defined when you compile the program.
 * @version 0.1
 * @date 2025-12-09
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <iostream>

#ifdef NUMBER
#if (NUMBER != 8) && (NUMBER != 16)
#error "NUMBER is not 8 or 16"
#endif

int main(void)
{
    std::cout << NUMBER << std::endl;

    return 0;
};
#endif

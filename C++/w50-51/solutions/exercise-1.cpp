/**
 * @file exercise-1.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a program to print a macro named NUMBER whose value is an integer to the terminal.
 *        If NUMBER has not been defined, an error message shall be generated and compilation shall be terminated.
 *        If NUMBER is not equal to 8 or 16,  an error message shall be generated and compilation shall be terminated.
 *        The macro shall be defined when you compile the program.
 *
 * @version 0.1
 * @date 2021-10-03
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <iostream>

int main(void)
{
#if !defined(NUMBER) || ((NUMBER != 8) && (NUMBER != 16))
#error NUMBER shall be defined and its value shall be 8 or 16
#else
    std::cout << "NUMBER = " << NUMBER << std::endl;
#endif
    return 0;
}

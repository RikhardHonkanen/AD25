/**
 * @file main.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of preprocessor directives
 * @version 0.1
 * @date 2021-02-07
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "mylib.h"  // Include a library developed by myself using double quotation marks
#include <iostream> // Include a standard library using < and >

#ifndef MY_MACRO // Check if MY_MACRO has been defined or not
#error "MY_MACRO has not been defined!" // If MY_MACRO is not defined the error message is printed to the terminal and the compilation is stopped
#endif // End of the conditional block

#define DEBUG // Define DEBUG as a macro

#define AREA(r) (PI * (r) * (r)) // A macro can be used in another macro. PI is used befor defining.
#define PI 3.1415                // PI is an object-like macro. Macros can be used as constants

#define SQRX(x) ((x) * (x)) // A function-like macro

// A Multi line macro
#define PRINT_MSG(msg)          \
    {                           \
        std::cout << msg        \
                  << std::endl; \
    }

#ifdef ARDUINO
#define print_log(fmt, ...) Serial.printf(fmt, __VA_ARGS__); // A macro with a variable number of arguments.
#else
#define print_log(fmt, ...) (void)printf(fmt, __VA_ARGS__); // A macro with a variable number of arguments.
#endif

// A macro with the stringify operator (#) and the spread operator (...).
#define print_args(...) std::cout << #__VA_ARGS__ << std::endl

#define print_exp(exp) std::cout << #exp " = " << exp << std::endl // An example of the stringify operator

#define TEXT_A "Hello, world!"
#define TEXT_B "I Love Programming."
#define print_msg(x) std::cout << TEXT_##x << std::endl // A macro with the concatenation operator (##)

int main(void)
{
    print();

    // And example of the defined operator
#if defined(__unix__) && defined(__GNUC__)
    std::cout << "GNU/Linux" << std::endl;
#endif

    // An example of conditional compilation
#ifdef DEBUG
    std::cout << "A debug message" << std::endl;
#endif

    // Some predefined macros
    std::cout << "Date: " << __DATE__ << std::endl;
    std::cout << "Func: " << __func__ << std::endl;

    std::cout << "Line: " << __LINE__ << std::endl;
#line 1000
    std::cout << "Line: " << __LINE__ << std::endl;

    // Using print_log as a macro with a variable number of arguments
    print_log("%d - %f\n", 10, 20.0f);
    print_log("%d - %f - %s\n", 10, 20.0f, "Test");

    // Using print_args to print the arguments to the terminal
    print_args(one\n, "2\n", three);

    // Using printEXP to print the expression and its result to the terminal
    print_exp(4 * 32 * 20);

    // Print MY_MACRO which is defined during compilation to the terminal
    std::cout << "My Macro: " << MY_MACRO << std::endl;

    // Using print_msg to print TEXT_A and TEXT_B
    // print_msg uses concatenation operator (##)
    print_msg(A);
    print_msg(B);

#define A 10 // Define A as 10

    std::cout << "A = " << A << std::endl; // Print A, which is 10, to the terminal

#undef A     // Undefine A
#define A 20 // Redefine A as 20

    std::cout << "A = " << A << std::endl; // Print A, which is 20, to the terminal

// The right way to comment code out
#if 0
    std::cout << "................" << std::endl;
    std::cout << "......ABCD......" << std::endl;
    std::cout << "................" << std::endl;
#endif

    return 0;
}
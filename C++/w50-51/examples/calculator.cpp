/**
 * @file calculator.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make program to evaluate an expression in the format of operand1 operator operand2.
 *        Type of the operands shall be int and the operator can be +, -, / or *.
 *        Print the evaluated value of the expression to the terminal.
 *
 *        Check all the possible errors and in the case of an error print an error message to the terminal.
 *
 *       Examples:
 *          Enter an expression: 12 / 23
 *          12 / 23 = 0.521739
 *
 * @version 0.1
 * @date 2022-10-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <iostream>

int main(void)
{
    char op;
    int a, b;

    std::cout << "Enter an expression: ";

    std::cin >> a >> op >> b;
    if (std::cin.good())
    {
        switch (op)
        {
        case '+':
            (void)printf("%d + %d = %d\n", a, b, a + b);
            break;

        case '-':
            (void)printf("%d - %d = %d\n", a, b, a - b);
            break;

        case '*':
            (void)printf("%d * %d = %d\n", a, b, a * b);
            break;

        case '/':
            if (b == 0)
            {
                std::cout << "Error: division by zero!" << std::endl;
            }
            else
            {
                std::cout << a << " / " << b << " = " << static_cast<float>(a) / b << std::endl;
            }
            break;

        default:
            std::cout << "Invalid operator!" << std::endl;
            break;
        }
    }
    else
    {
        std::cout << "Invalid expression!" << std::endl;
    }

    return 0;
}
/**
 * @file calculator.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Write a c program to evaluate an expression in the format of operand1 operator operand2.
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
#include <stdio.h>

int main(void)
{
    int a, b;
    char operator;

    (void)printf("Enter an expression: ");

    if (3 == scanf("%d %c %d", &a, &operator, & b))
    {
        switch (operator)
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
                (void)printf("Error: division by zero!\n");
            }
            else
            {
                (void)printf("%d / %d = %g\n", a, b, (float)a / b);
            }
            break;

        default:
            (void)printf("Invalid operator!\n");
            break;
        }
    }
    else
    {
        (void)printf("Invalid expression!\n");
    }

    return 0;
}
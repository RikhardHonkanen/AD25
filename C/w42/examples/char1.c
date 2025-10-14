/**
 * @file char.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Read a character(chr) from a terminal using scanf.
 *        If chr is a digit then convert it to a number and print the number to the terminal.
 *        else if chr is an uppercase letter then convert it to lowercase and print it to the terminal.
 *        else if chr is a lowercase letter then convert it to uppercase and print it to the terminal.
 *        otherwise print chr to the terminal.
 *
 *        - If scanf cannot read a character print an error message to the terminal
 *
 * @version 0.1
 * @date 2022-10-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>

int main(void)
{
    char chr;

    (void)printf("Enter a character: ");

    if (1 == scanf("%c", &chr))
    {
        if ((chr >= '0') && (chr <= '9'))
        {
            (void)printf("Number: %d\n", chr - '0');
        }
        else if ((chr >= 'a') && (chr <= 'z'))
        {
            (void)printf("%c\n", chr - ('a' - 'A'));
        }
        else if ((chr >= 'A') && (chr <= 'Z'))
        {
            (void)printf("%c\n", chr + ('a' - 'A'));
        }
        else
        {
            (void)printf("Character: %c\n", chr);
        }
    }
    else
    {
        (void)printf("Failed to read a char!\n");
    }

    return 0;
}
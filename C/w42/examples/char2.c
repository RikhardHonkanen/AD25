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
#include <ctype.h>

int main(void)
{
    char chr;

    (void)printf("Enter a character: ");

    if (1 == scanf("%c", &chr))
    {
        if (isdigit(chr))
        {
            (void)printf("Number: %d\n", chr - '0');
        }
        else if (isalpha(chr))
        {
            if (islower(chr))
            {
                (void)printf("%c\n", toupper(chr));
            }
            else
            {
                (void)printf("%c\n", tolower(chr));
            }
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
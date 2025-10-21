/**
 * @file string-1.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of string.
 *        Write a program to read a string of max. 32 characters and
 *        print only letters in the string to the terminal.
 *
 *        Example:
 *           Enter a string: G9-aU*b
 *           Only letters: GaUb
 *
 * @version 0.1
 * @date 2022-10-26
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define STR_MAX_LEN 32

int main(void)
{
    char string[STR_MAX_LEN + 1] = {0};

    (void)printf("Enter a string: ");
    (void)fgets(string, sizeof(string), stdin); // fgets may capture a new line.

    int len = strlen(string);
    if (len < STR_MAX_LEN)
    {
        len--;
        string[len] = '\0';
    }

    (void)printf("Only letters: ");
    for (int i = 0; i < len; i++)
    {
        if (isalpha(string[i]))
        {
            (void)putchar(string[i]);
        }
    }

    (void)putchar('\n');

    return 0;
}
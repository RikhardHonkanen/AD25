/**
 * @file string-2.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of string.
 *        Write a program to read a string of max. 32 characters.
 *        Remove the non-letter characters from the string and then
 *        print it to the terminal.
 *
 *        Example:
 *           Enter a string: G9-aU*b
 *           The filtered string: GaUb
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

    for (int i = 0; i < STR_MAX_LEN; i++)
    {
        char chr = getchar();
        if ((chr == '\n') || (chr == EOF))
        {
            break;
        }
        string[i] = chr;
    }

    int len = strlen(string);
    char temp[STR_MAX_LEN + 1] = {0};

    for (int i = 0, j = 0; i < len; i++)
    {
        if (isalpha(string[i]))
        {
            temp[j] = string[i];
            j++;
        }
    }

    (void)strncpy(string, temp, sizeof(string));

    (void)printf("The filtered string: %s\n", string);

    return 0;
}
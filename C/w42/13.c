/**
 * @file 13.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief  Read a full name character by character from the standard input.
 * The program shall filter, capitalize and then print it out to the standard output (terminal).
 * The filtered name can only contain one space between parts of the name and uppercase/lowercase letters (a-z).
 * Don’t use an array to store the name. The entered name shall be handled character by character.
 * @version 0.1
 * @date 2025-10-14
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <ctype.h>
int main(void)
{
    int whitespace_used = 0;
    int capitalize = 1;
    int c; // must be int to handle EOF

    (void)printf("\nEnter your name: ");
    do
    {
        c = getchar();
        if (!isalpha(c) && !isspace(c))
        {
            continue;
        }
        if (isspace(c))
        {
            if (whitespace_used == 1)
            {
                continue;
            }
            else
            {
                whitespace_used = 1;
                capitalize = 1;
                putchar(c);
                continue;
            }
        }
        if (capitalize == 1)
        {
            putchar(toupper(c));
            capitalize = 0;
            continue;
        }
        putchar(tolower(c));
    } while (c != '\n');

    return 0;
};
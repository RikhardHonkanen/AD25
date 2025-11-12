/**
 * @file exercise-18.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a program to read a string from the standard input,
 *        then split the string by spaces as the delimiter and
 *        print the substrings to the output. For example:
 *                                  The input string: Hello  World
 *                                  The output:
 *                                          Hello
 *                                          World
 * @version 0.1
 * @date 2021-02-23
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define LENGTH 64

int main(void)
{
    char string[LENGTH + 1] = {0};

    // Read the string from the standard input
    (void)printf("Enter a string: ");
    (void)fgets(string, sizeof(string), stdin); // fgets may capture a newline
    int length = strlen(string);
    if (string[length - 1] == '\n')
    {
        length--;
        string[length] = '\0';
    }

    int i = 0;
    bool space = false;

    while (i < length)
    {
        if (isspace(string[i]))
        {
            if (!space)
            {
                space = true;
            }
        }
        else
        {
            if (space)
            {
                space = false;
                (void)putchar('\n');
            }
            (void)putchar(string[i]);
        }
        i++;
    }

    return 0;
}

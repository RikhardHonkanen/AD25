/**
 * @file exercise-19.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a program to read date and time from a terminal and then
 *        check if the entered date and time is in the format of YYYY-MM-DD HH:mm:ss or not.
 *
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

#define FORMAT "YYYY-MM-DD hh:mm:ss"

int main(void)
{
    const char *format = FORMAT;
    char string[sizeof(FORMAT) + 1] = {0};

    // Read the string from the standard input
    (void)printf("Enter date and time in the format of %s: ", FORMAT);
    (void)fgets(string, sizeof(string), stdin);
    int length = strlen(string);
    if (string[length - 1] == '\n')
    {
        length--;
        string[length] = '\0';
    }

    int i = 0;
    bool is_correct = true;

    while (i < sizeof(FORMAT) - 1)
    {
        if (isalpha(format[i]))
        {
            if (!isdigit(string[i]))
            {
                is_correct = false;
                break;
            }
        }
        else
        {
            if (format[i] != string[i])
            {
                is_correct = false;
                break;
            }
        }
        i++;
    }

    (void)printf("The format of the entered date and time is%s correct!\n", is_correct ? "" : " not");

    return 0;
}

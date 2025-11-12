/**
 * @file exercise-20.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a program to read a string from a terminal and then convert
 *        the uppercase letters to lowercase and vice versa and
 *        in the end print the string to the standard output.
 *
 *        *** To change and print the string use only pointers ***
 *
 * @version 0.1
 * @date 2021-03-05
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define STR_LEN 63

int main(void)
{
    char string[STR_LEN + 1] = {0};

    // Read the string from a terminal
    int i = 0;
    (void)printf("Enter a string: ");
    while (i < STR_LEN)
    {
        char chr = getchar();
        if ((chr == '\n') || (chr == EOF))
        {
            break;
        }
        string[i++] = chr;
    }

    // Change cases of the letters
    for (char *cptr = string; *cptr != '\0'; cptr++)
    {
        *cptr = isupper(*cptr) ? tolower(*cptr) : toupper(*cptr);
    }

    (void)printf("The converted string: %s\n", string);

    return 0;
}

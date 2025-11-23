/**
 * @file 20.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Program to read a string from a terminal and then convert
 * the uppercase letters to lowercase and vice versa and
 * in the end print the string to the standard output.
 * To change and print the string use only pointers.
 * @version 0.1
 * @date 2025-10-21
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <ctype.h>

int main(void)
{
    char mystring[100];

    (void)printf("Enter a string:\n");
    if (fgets(mystring, 100, stdin) != NULL)
    {
        for (char *iptr = mystring; (iptr - mystring) < sizeof(mystring) / sizeof(*mystring); iptr++)
        {
            if (*iptr == '\0' || *iptr == "\n" || *iptr == EOF)
            {
                break;
            }
            if (islower(*iptr))
            {
                putchar(toupper(*iptr));
            }
            else if (isupper(*iptr))
            {
                putchar(tolower(*iptr));
            }
            else
            {
                putchar(*iptr);
            }
        }
    }

    return 0;
};
/**
 * @file 18.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Program to read a string from the standard input,
 * then split the string by spaces as the delimiter and print the substrings to the output.
 * @version 0.1
 * @date 2025-10-21
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <string.h>

int main(void)
{
    char mystring[100];

    (void)printf("Enter a string:\n");
    if (fgets(mystring, 100, stdin) != NULL)
    {
        (void)printf("Raw: %s\n", mystring);

        // Returns first token
        char *token = strtok(mystring, " ");

        // Keep printing tokens while one of the
        // delimiters present in str[].
        while (token != NULL)
        {
            (void)printf("%s\n", token);
            token = strtok(NULL, " ");
        }
    }

    return 0;
};
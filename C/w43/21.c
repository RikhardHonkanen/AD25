/**
 * @file 21.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Program to read date and time in the format of YYYY-MM-DD HH:mm:ss from the input and validate it.
 * A leap year is exactly divisible by 4 except for century years (years ending with 00).
 * The century year is a leap year only if it is perfectly divisible by 400.
 * If the year is a leap year, then February 29, is the leap day.
 * @version 0.1
 * @date 2025-10-21
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <time.h>

int main(void)
{
    char mystring[100];
    struct tm tm;
    char buf[255];

    (void)printf("Enter a date in format YYYY-MM-DD HH:mm:ss:\n");
    if (fgets(mystring, 100, stdin) != NULL)
    {

        // Parsing the date string into struct tm
        if (strptime(mystring, "%Y-%m-%d", &tm) == NULL)
        {
            printf("Failed to parse date.\n");
        }
        else
        {
            // Using strftime to print the date in a readable format
            strftime(buf, sizeof(buf), "%A, %B %d, %Y", &tm);
            printf("Parsed date: %s\n", buf);
        }
    }

    return 0;
};
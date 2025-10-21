/**
 * @file 19.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Program to read date and time from a terminal and then
 * check if the entered date and time is in the format of YYYY-MM-DD HH:mm:ss or not.
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
    time_t mydate;

    (void)printf("Enter a datetime in the format YYY-MM-DD HH:mm:ss:\n");
    if (fgets(mystring, 100, stdin) != NULL)
    {
        (void)printf("Raw: %s\n", mystring);
    }

    return 0;
};
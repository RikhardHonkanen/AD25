/**
 * @file exercise-32.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a program to get a positive integer as an argument from the command line
 *        and then check if the number is prime or not.
 *
 *        To compile and run: gcc exercise-35.c -lm -o main && ./main 123
 *
 * @version 0.1
 * @date 2022-11-05
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define FIRST_PRIME 2U

static bool strtouint(const char *str, uint32_t *uiptr)
{
    bool status = false;

    if (uiptr != NULL)
    {
        *uiptr = 0;
        status = true;

        for (char *ptr = (char *)str; *ptr != '\0'; ptr++)
        {
            if ((*ptr < '0') || (*ptr > '9'))
            {
                status = false;
                break;
            }

            *uiptr *= 10;
            *uiptr += (*ptr - '0');
        }
    }

    return status;
}

static bool isprime(uint32_t num)
{
    bool prime = true;

    if (num < FIRST_PRIME)
    {
        prime = false;
    }
    else
    {
        if (num > FIRST_PRIME)
        {
            uint8_t counter = FIRST_PRIME;
            uint8_t max_number = ceil(sqrt(num));

            while (counter <= max_number)
            {
                if (num % counter == 0)
                {
                    prime = false;
                    break;
                }

                counter++;
            }
        }
    }

    return prime;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        (void)printf("Invalid number of arguments.\n");
        exit(1);
    }

    uint32_t number;

    if (!strtouint(argv[1], &number))
    {
        (void)printf("Invalid argument. The argument shall be an unsigned integer.\n");
        exit(1);
    }

    (void)printf("%u is %sprime!\n", number, isprime(number) ? "" : "not ");

    return 0;
}
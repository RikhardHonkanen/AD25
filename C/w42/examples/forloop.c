/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of for loop:
 *        Print 5 capitalized (first letter uppercase and the other letters lowercase) random strings in the terminal.
 *        A random string shall only contain letters (a-z and A-z).
 *        Length of a random string shall be a random number in the range of 2 and 8.
 *
 *        No magic numbers!
 *
 *        Example:
 *           Asdh
 *           Bmkdsds
 *           Cpopsp
 *           Dksdsk
 *           Eefansj
 *
 * @version 0.1
 * @date 2022-10-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define STR_NUM 5
#define LEN_MIN 2
#define LEN_MAX 8
#define ALPHA_NUM 26

int main(void)
{
    srand(time(NULL));

    for (int i = 0; i < STR_NUM; ++i)
    {
        int len = LEN_MIN + rand() % (LEN_MAX - LEN_MIN + 1);

        putchar('A' + rand() % ALPHA_NUM);

        for (int j = 1; j < len; j++)
        {
            putchar('a' + rand() % ALPHA_NUM);
        }

        putchar('\n');
    }

    return 0;
}
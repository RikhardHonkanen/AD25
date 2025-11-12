/**
 * @file exercise-4.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Display size. min and max of the basic data types
 * @version 0.1
 * @date 2021-10-03
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <stdio.h>

#if !defined(NUMBER) || ((NUMBER != 8) && (NUMBER != 16))
#error NUMBER shall be defined and its value shall be 8 or 16
#endif

int main(void)
{
    printf("NUMBER = %d\n", NUMBER);

    return 0;
}

/**
 * @file exercise-11.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a program to read a number in the range of 0 and 255 from the standard input using the scanf function in stdio.h
 *        and print the binary form of the number to the output. While the enetered number is not in the range,
 *        the program shall ask for a new number.
 * @version 0.1
 * @date 2021-02-21
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <stdio.h>

#define BYTE_BITS 8
#define RANGE_MIN 0
#define RANGE_MAX 255

int main(void)
{
    int number = 0;

    do
    {
        (void)printf("Enter a number in the range of %d and %d: ", RANGE_MIN, RANGE_MAX);
        (void)scanf("%d", &number);
    } while ((number < RANGE_MIN) || (number > RANGE_MAX));

    for (int i = BYTE_BITS - 1; i >= 0; --i)
    {
        (void)printf("%d", (number >> i) & 1);
    }

    (void)printf("\n");

    return 0;
}

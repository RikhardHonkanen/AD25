/**
 * @file exercise-7.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief 1) Make an instance of date_t
 *        2) Read day, month and year from a terminal
 *        3) Print the instance to the terminal in the formate of YYYY-MM-DD
 * @version 0.1
 * @date 2021-02-19
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <stdio.h>
#include <stdint.h>

typedef struct
{
    int day;
    int month;
    int year;

} date_t;

int main(void)
{
    date_t date = {0};

    (void)printf("Enter year, month, day: ");

    (void)scanf("%d, %d, %d", &date.year, &date.month, &date.day);

    (void)printf("The date: %04d-%02d-%02d\n", date.year, date.month, date.day);

    return 0;
}
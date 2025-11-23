/**
 * @file 35.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Read about the hanoi tower puzzle and solve it for 64 disks. https://romek.info/games/hanoi3e.html
 *
 * Make a program to find the number of years it takes to move 64 disks from a peg to another peg if each move takes 1 second.
 * @version 0.1
 * @date 2025-11-02
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>
#include <math.h>

int main(void)
{
    double moves = pow(2, 64) - 1; // Loss of precision, pow prototybe returns a double. More accurate is UINT64_MAX;
    double seconds_per_year = 60.0 * 60.0 * 24.0 * 365.0;
    double years = moves / seconds_per_year;

    printf("It would take approximately %.2e years.\n", years);
    return 0;
}
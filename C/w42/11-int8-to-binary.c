/**
 * @file 11.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief read a number in the range of 0 and 255 from the standard input
 * using the scanf function in stdio.h and print the binary form of the number
 * to the output. While the enetered number is not in the range, the program shall ask a new number.
 * @version 0.1
 * @date 2025-10-14
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <stdio.h>

int main(void)
{
    int num = 0;

    do
    {
        (void)printf("\nEnter a number between 0 and 255: ");
        (void)scanf(" %d", &num);
    } while (num < 0 || num > 255);

    // sizeof(n) * 8 → total bits in the integer.
    //(n >> i) & 1 → shifts the bit at position i to the least significant bit and masks it.
    for (int i = sizeof(num) * 8 - 1; i >= 0; i--)
    {
        printf("%d", (num >> i) & 1);
    }
    printf("\n");

    return 0;
};

// void print_binary(unsigned int n) {
//     for (int i = sizeof(n) * 8 - 1; i >= 0; i--) {
//         printf("%d", (n >> i) & 1);
//     }
//     printf("\n");
// }
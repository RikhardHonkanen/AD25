/**
 * @file 15.c
 * @author Rikhard Honkanen (rikhard.honkanen@mail.com)
 * @brief Get a string from the standard input, then print the string to the output in the reversed order.
 * @version 0.1
 * @date 2025-10-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <stdio.h>

void rev(char arr[], int n)
{
    // Two pointers
    int l = 0, r = n - 1;
    while (l < r)
    {
        // Swap the elements
        char temp = arr[l];
        arr[l] = arr[r];
        arr[r] = temp;

        // Move pointers towards middle
        l++;
        r--;
    }
}

int main(void)
{
    char in[6] = {0};
    printf("Enter a string (6 characters): ");
    gets(in); // avoid this (not safe), use fgets()

    printf("Original: %s\n", in);
    size_t size = sizeof(in) / sizeof(in[0]);
    rev(in, size);
    printf("Reversed: %s", in);

    return 0;
};
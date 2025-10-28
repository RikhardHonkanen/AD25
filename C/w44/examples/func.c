/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of function.
 *        1. Make a function(print_string) to take a string and print it to the terminal.
 *        2. Make a function pointer to print_string and then in your program call print_string
 *           via the function pointer in order to print your name to the terminal.
 *
 * @version 0.1
 * @date 2022-11-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>

void print_string(char *str); // Declaration of print_string

int main(void)
{
    void (*fptr)(char *) = print_string; // fptr is a function pointer initialized by print_string

    fptr("Faroch"); // Calls print_string by passing Faroch to it

    return 0;
}

// Definition of print_string
void print_string(char *str)
{
    (void)printf("%s\n", str);
}
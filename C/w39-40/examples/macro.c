// Make a macro to define a multiply function like macro to multiply two integers
// Using the macro to print multiplication of 3 and 5 to the terminal

#include <stdio.h>

#define MULTIPLY(x, y) ((x) * (y))

int main(void)
{
    printf("Result: %d\n", MULTIPLY(3, 5));

    return 0;
}
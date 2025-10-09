#include <stdio.h>

/**
 * @brief
 * Reads a date and prints it to console
 *
 * @return int
 */

union
{
    int i;
    float f;
} u;

int main(void)
{
    // Convert floating-point bits to integer:
    u.f = 3.14159f;
    printf("As integer: %08x\n", u.i);
    return 0;
}
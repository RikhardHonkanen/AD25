#include <stdio.h>
/**
 * @brief
 * Will compile if the number provided by -DNUMBER is 8 or 16.
 *
 * @return int
 */
int main(void)
{
#ifdef NUMBER
#if (NUMBER != 8) && (NUMBER != 16)
#error "NUMBER is not 8 or 16"
#endif
#else
#error "NUMBER is not defined!"
#endif
    printf("Done!");
    return 0;
}
#include <stdio.h>

#if !defined(NUMBER)
#error "NUMBER is not defined!"
#elif (NUMBER != 8) && (NUMBER != 16)
#error "NUMBER must be 8 or 16"
#endif

int main(void)
{
    printf("Done!\n");
    return 0;
}
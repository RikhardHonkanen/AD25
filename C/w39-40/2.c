
#include <stdio.h>

int main(void)
{
#ifdef USERNAME
    printf("%s \n", USERNAME);
#endif
    return 0;
}
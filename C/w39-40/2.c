
#include <stdio.h>
// #define USERNAME ""

int main(void)
{
// printf("Test");
#ifdef USERNAME
    printf("%s \n", USERNAME);
#endif
    return 0;
}
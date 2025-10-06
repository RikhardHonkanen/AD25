#include <stdio.h>
#include <stdalign.h>

alignas(16) char a, b;
char c;

int main(void)
{
    printf("%p\n", (void *)&a);
    printf("%p\n", (void *)&b);
    printf("%p\n", (void *)&c);
}
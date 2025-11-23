
#include <stdio.h>

/**
 * @brief
 * Prints name provided by -DUSERNAME
 *
 * @return int
 */
int main(void)
{
#ifdef USERNAME
    printf("%s \n", USERNAME);
#endif
    return 0;
}
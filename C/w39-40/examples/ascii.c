#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf(">> Print\tHorizontalTAB\n");
    printf(">> Print\vVerticalTAB\n");
    printf(">> Print\fFormFeed\n");
    printf(">> Print\na new line.\n");
    printf(">> Printt\bBackspace\n");
    printf(">> Print special characters: %% ' \" \\ ?\n");

    for (int i = 0; i < 15; i++)
    {
        printf("\r%d", i);
        fflush(stdout);
        usleep(100000);
    }
    printf("\nRing the bell! \a\n");

    return 0;
}
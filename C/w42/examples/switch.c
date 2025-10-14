#include <stdio.h>

int main(void)
{
    printf("A) Add a file\n");
    printf("E) Edit a file\n");
    printf("D) Delete a file\n");
    printf("Enter a command: ");

    char command = getchar();

    switch (command)
    {
    case 'a':
    case 'A':
        printf("The file added!\n");
        break;

    case 'e':
    case 'E':
        printf("The file edited!\n");
        break;

    case 'd':
    case 'D':
        printf("The file deleted!\n");
        break;

    default:
        printf("Invalid command\n");
        break;
    }

    return 0;
}
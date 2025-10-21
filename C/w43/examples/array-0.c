#include <stdio.h>

#define ROWS 2
#define COLUMNS 3

int main(void)
{
    int matrix[ROWS][COLUMNS] = {[0][1] = 100, [1] = {4, 5, 6}};

    (void)printf("%d\n", sizeof(matrix));

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            (void)printf("matrix[%d][%d] = %d\n", i, j, matrix[i][j]);
        }
    }

    return 0;
}
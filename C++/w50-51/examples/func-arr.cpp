#include <iostream>

constexpr int M{2};
constexpr int N{3};

static void print0(int length, const int arr[])
{
    for (int i = 0; i < length; i++)
    {
        (void)printf("array[%d] = %d\n", i, arr[i]);
    }
}

static void print1(int length, const int arr[])
{
    for (int i = 0; i < length; i++)
    {
        (void)printf("array[%d] = %d\n", i, arr[i]);
    }
}

static void print2(int length, const int *arr)
{
    for (int i = 0; i < length; i++)
    {
        (void)printf("array[%d] = %d\n", i, arr[i]);
    }
}

static void print3(size_t m, size_t n, const int matrix[][N])
{
    for (size_t i = 0; i < m; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            (void)printf("%d\t", matrix[i][j]);
        }
        (void)printf("\n");
    }
}

static void print4(size_t m, size_t n, const int (*matrix)[N])
{
    for (size_t i = 0; i < m; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            (void)printf("%d\t", matrix[i][j]);
        }
        (void)printf("\n");
    }
}

static void print5(size_t m, size_t n, const int (*matrix)[])
{
    const int (*ptr)[n] = (const int (*)[n])matrix;

    for (size_t i = 0; i < m; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            (void)printf("%d\t", ptr[i][j]);
        }
        (void)printf("\n");
    }
}

int main(void)
{
    int array[M * N] = {1, 2, 3, 4, 5, 6};

    print0(sizeof(array) / sizeof(*array), array);
    (void)printf("\n");

    print1(sizeof(array) / sizeof(*array), array);
    (void)printf("\n");

    print2(sizeof(array) / sizeof(*array), array);
    (void)printf("\n");

    int matrix[M][N] = {{1, 2, 3}, {4, 5, 6}};

    print3(M, N, matrix);
    (void)printf("\n");

    print4(M, N, matrix);
    (void)printf("\n");

    print5(M, N, matrix);
    (void)printf("\n");

    return 0;
}

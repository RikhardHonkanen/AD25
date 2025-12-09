#include <ctime>
#include <cstdlib>
#include <iostream>

constexpr int M{2};
constexpr int N{3};

int main(void)
{
    srand(time(nullptr));

    int matrix[M][N]{};
    for (int row = 0; row < M; row++)
    {
        for (int col = 0; col < N; col++)
        {
            matrix[row][col] = std::rand() % 100;
        }
    }

    for (int row = 0; row < M; row++)
    {
        for (int col = 0; col < N; col++)
        {
            std::cout << matrix[row][col] << "\t";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;

    int array[N]{};

    for (int i = 0; i < N; i++)
    {
        array[i] = std::rand() % 100;
    }

    for (const int elem : array)
    {
        std::cout << elem << std::endl;
    }

    for (int i{1}; i < 11; i++)
    {
        for (int j{1}; j < 11; j++)
        {
            std::cout << i * j << '\t';
        }
        std::cout << std::endl;
    }

    return 0;
}
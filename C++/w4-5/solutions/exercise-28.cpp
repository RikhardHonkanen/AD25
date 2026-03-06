/**
 * @file exercise-28.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Generate and store 1000 random numbers in the range of 100 and 100000 in a vector,
 *        then remove the non-prime numbers from the vector and in the end usinng an iterator
 *        print the elements in the vector (in ascending order) to the terminal.
 *
 * @version 0.1
 * @date 2021-06-18
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <cmath>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>

constexpr int NUMBER{1000};
constexpr int FIRST_PRIME{2};
constexpr int MIN_RANGE{100};
constexpr int MAX_RANGE{100000};

static bool isnonprime(int num);

int main(void)
{
    srand(time(nullptr));

    std::vector<int> vec;

    for (int i = 0; i <= NUMBER; i++)
    {
        vec.push_back(rand() % (MAX_RANGE - MIN_RANGE) + MIN_RANGE);
    }

    vec.erase(std::remove_if(vec.begin(), vec.end(), isnonprime), vec.end());

    std::sort(vec.begin(), vec.end(), [](int a, int b)
              { return (a < b); });

    for (const auto elem : vec)
    {
        std::cout << elem << std::endl;
    }

    std::cout << std::endl;

    return 0;
}

static bool isnonprime(int num)
{
    bool status{true};

    if (num >= FIRST_PRIME)
    {
        status = false;

        if (num > FIRST_PRIME)
        {
            status = true;

            const int max_num = std::ceil(std::sqrt(num));
            for (int i = FIRST_PRIME; i <= max_num; i++)
            {
                if (num % i == 0)
                {
                    status = false;
                    break;
                }
            }

            status = !status;
        }
    }

    return status;
}

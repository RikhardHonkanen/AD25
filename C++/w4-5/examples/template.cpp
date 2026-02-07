/**
 * @file template.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a template function to fill an array of T with random number numbers in the range of 0.0 and 1.0.
 *        The function shall be instantiable if T is a floating point type
 *
 * @version 0.1
 * @date 2022-12-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iomanip>
#include <iostream>

#if 1
template <typename T, size_t SIZE>
void func(void)
{
    static_assert(std::is_floating_point_v<T> && (SIZE > 0));

    T array[SIZE]{0};
#else
template <typename T, size_t SIZE, typename = std::enable_if_t<std::is_floating_point<T>::value>>
void func(void)
{
    T array[SIZE]{0};
#endif
    std::srand(std::time(nullptr));

    for (auto &elem : array)
    {
        elem = std::rand() / T(RAND_MAX);
    }

    for (const auto &elem : array)
    {
        std::cout << std::setprecision(6) << std::fixed << elem << std::endl;
    }
}

int main(void)
{
    func<float, 5>();

    return 0;
}

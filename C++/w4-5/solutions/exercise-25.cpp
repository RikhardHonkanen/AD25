/**
 * @file exercise-25.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a template function named map to map a value in a range to a value in another range.
 *        Type of the ranges shall be arithmetic types.
 *        Example: 0.5f in the range of 0.0f and 3.3f is mapped to 155 in the range of 0 and 1023
 *
 * @version 0.1
 * @date 2022-12-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>

template <typename T>
struct Range
{
    T min;
    T max;
};

#if 1
template <typename F, typename T>
#if 0
std::enable_if_t<std::is_arithmetic<T>::value, T> map(std::enable_if_t<std::is_arithmetic<F>::value, F> value, Range<F> from, Range<T> to)
{
#else
T map(F value, Range<F> from, Range<T> to)
{
    static_assert(std::is_arithmetic_v<F> && std::is_arithmetic_v<T>);
#endif
#else
template <typename F, typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value && std::is_arithmetic<F>::value>>
T map(F value, Range<F> from, Range<T> to)
{
#endif

    return (((to.max - to.min) / (from.max - from.min)) * (value - from.min) + to.min);
}

int main(void)
{
    Range<float> from{0.0f, 3.3f};
    Range<int> to{0, 1023};

    std::cout << map(0.5f, from, to) << std::endl;

    return 0;
}

#include <cassert>
#include <cstring>
#include <iostream>
#include "fizzbuzz.h"

void test_fizz_buzz_returns_nothing(void)
{
    assert(0 == std::strcmp("", fizz_buzz(0)));
    std::cout << "[Passed] test_fizz_buzz_returns_nothing" << std::endl;
}

void test_fizz_buzz_returns_digits(void)
{
    assert(0 == std::strcmp("1", fizz_buzz(1)));
    std::cout << "[Passed] test_fizz_buzz_returns_digits" << std::endl;
}

void test_fizz_buzz_returns_fizz(void)
{
    assert(0 == std::strcmp("Fizz", fizz_buzz(3)));
    std::cout << "[Passed] test_fizz_buzz_returns_fizz" << std::endl;
}

void test_fizz_buzz_returns_buzz(void)
{
    assert(0 == std::strcmp("Buzz", fizz_buzz(10)));
    std::cout << "[Passed] test_fizz_buzz_returns_buzz" << std::endl;
}

void test_fizz_buzz_returns_fizzbuzz(void)
{
    assert(0 == std::strcmp("FizzBuzz", fizz_buzz(15)));
    std::cout << "[Passed] test_fizz_buzz_returns_fizzbuzz" << std::endl;
}

int main(void)
{
    test_fizz_buzz_returns_nothing();
    test_fizz_buzz_returns_digits();
    test_fizz_buzz_returns_fizz();
    test_fizz_buzz_returns_buzz();
    test_fizz_buzz_returns_fizzbuzz();

    return 0;
}

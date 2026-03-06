#include <cassert>
#include "person.h"
#include <iostream>

int main(void)
{
    std::string name{};

    assert(0 == person::get_age());
    assert("" == person::get_name());
    std::cout << "Testing uninit... Passed" << std::endl;

    person::init("Eva", 21);

    assert(21 == person::get_age());
    assert("Eva" == person::get_name());
    std::cout << "Testing init... Passed" << std::endl;

    person::set_name("Stefan");
    assert("Stefan" == person::get_name());
    std::cout << "Testing set name... Passed" << std::endl;

    person::set_age(30);
    assert(30 == person::get_age());
    std::cout << "Testing set age... Passed" << std::endl;

    std::cout << "---------------------------" << std::endl;
    std::cout << "All tests Passed!" << std::endl;

    return 0;
}
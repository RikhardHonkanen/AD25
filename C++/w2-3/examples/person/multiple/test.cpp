#include <cassert>
#include "person.h"
#include <iostream>

int main(void)
{
    person_t *stefan = person::create("Stefan", 30);
    assert(stefan != nullptr);

    assert(30 == person::get_age(stefan));
    assert("Stefan" == person::get_name(stefan));
    std::cout << "Testing create stefan... Passed" << std::endl;

    person_t *eva = person::create("Eva", 30);
    assert(eva != nullptr);

    assert(30 == person::get_age(eva));
    assert("Eva" == person::get_name(eva));
    std::cout << "Testing create eva... Passed" << std::endl;

    person::set_name(eva, "Eva Larsson");
    assert("Eva Larsson" == person::get_name(eva));
    std::cout << "Testing set name... Passed" << std::endl;

    person::destroy(eva);
    assert(eva == nullptr);
    person::destroy(stefan);
    assert(stefan == nullptr);
    std::cout << "Testing destroy... Passed" << std::endl;

    std::cout << "---------------------------" << std::endl;
    std::cout << "All tests Passed!" << std::endl;

    return 0;
}
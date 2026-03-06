#include "buffer.h"
#include <cassert>
#include <iostream>

int main(void)
{
    int ival;
    float fval;

    std::cout << "Testing create an instance ... ";
    buffer_t *buf = buffer::create(8);
    assert(buf != nullptr);
    std::cout << "Passed" << std::endl;

    std::cout << "Testing insert and extract a positive float number ... ";
    assert(buffer::insert(buf, 4, 12, 10.5f));
    assert(buffer::extract(buf, 4, 12, fval));
    assert(fval == 10.5f);
    std::cout << "Passed" << std::endl;

    std::cout << "Testing insert and extract a negative float number ... ";
    assert(buffer::insert(buf, 4, 12, -10.5f));
    assert(buffer::extract(buf, 4, 12, fval));
    assert(fval == -10.5f);
    std::cout << "Passed" << std::endl;

    std::cout << "Testing insert and extract a positive integer number ... ";
    assert(buffer::insert(buf, 4, 12, 1000));
    assert(buffer::extract(buf, 4, 12, ival));
    assert(ival == 1000);
    std::cout << "Passed" << std::endl;

    std::cout << "Testing insert and extract a negative integer number ... ";
    assert(buffer::insert(buf, 4, 12, -1));
    assert(buffer::extract(buf, 4, 12, ival));
    assert(ival == -1);
    std::cout << "Passed" << std::endl;

    std::cout << "All tests passed" << std::endl;

    return 0;
}
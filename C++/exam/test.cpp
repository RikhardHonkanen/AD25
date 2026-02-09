#include "buffer.h"
#include <cassert>
#include <iostream>

int main(void) {
    try
    {
        Buffer<int> buffer{2};
    }
    catch (const std::exception &exp)
    {
        std::cout << exp.what() << ": passed!" << std::endl;
    }

    Buffer<int> buffer{4};
    assert(4 == buffer.capacity());
    assert(0 == buffer.available());
    std::cout << "Creating a buffer: passed!" << std::endl;

    assert(buffer.push(10));
    assert(1 == buffer.available());
    assert(buffer.push(20));
    assert(2 == buffer.available());
    assert(buffer.push(30));
    assert(3 == buffer.available());
    assert(buffer.push(40));
    assert(4 == buffer.available());
    assert(buffer.push(50));
    assert(4 == buffer.available());
    std::cout << "Push to buffer: passed!" << std::endl;

    assert(true == buffer.full());
    std::cout << "Assert full buffer: passed!" << std::endl;

    std::cout << buffer;

    assert(35 == buffer.average());
    std::cout << "Calculate buffer average: passed!" << std::endl;

    int item;
    assert(buffer.pop(item));
    assert(50 == item);
    assert(buffer.pop(item));
    assert(40 == item);
    assert(buffer.pop(item));
    assert(30 == item);
    std::cout << "Pop from buffer: passed!" << std::endl;

    buffer.clear();
    assert(0 == buffer.available());
    std::cout << "Clear buffer: passed!" << std::endl;


    return 0;
}
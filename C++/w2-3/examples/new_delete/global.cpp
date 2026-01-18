#include <iostream>

void *operator new(size_t size, const std::nothrow_t &tag) noexcept
{
    std::cout << "The overloaded new" << std::endl;
    return std::malloc(size);
}

void operator delete(void *ptr) noexcept
{
    std::cout << "The overloaded delete" << std::endl;
    std::free(ptr);
}

int main(void)
{
    int *ptr{new (std::nothrow) int{5}};

    std::cout << *ptr << std::endl;

    delete ptr;

    return 0;
}
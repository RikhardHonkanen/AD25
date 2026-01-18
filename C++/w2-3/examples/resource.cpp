#include <iostream>

class Resource
{
    int *iptr;

public:
    Resource(int value) : iptr{new(std::nothrow) int{value}} {}
    operator bool() { return (iptr != nullptr); }
    int getValue(void) { return *iptr; }
    ~Resource() { delete iptr; }
};

int main(void)
{
    Resource res{10};

    if (res)
    {
        std::cout << res.getValue() << std::endl;
    }

    return 0;
}
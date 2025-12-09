#include <limits>
#include <iostream>

int main(void)
{
    std::cout << "int size = " << sizeof(int) << std::endl;
    std::cout << "int min = " << std::numeric_limits<int>::min() << std::endl;
    std::cout << "int max =  " << std::numeric_limits<int>::max() << std::endl;

    std::cout << std::endl;

    std::cout << "long int size = " << sizeof(long int) << std::endl;
    std::cout << "long int min = " << std::numeric_limits<long int>::min() << std::endl;
    std::cout << "long int max =  " << std::numeric_limits<long int>::max() << std::endl;

    std::cout << std::endl;

    return 0;
}
// To build and run: g++ buffer/buffer.o  main.cpp -I./buffer -o main && ./main
#include <climits>
#include "buffer.h"
#include <iostream>

void print(uint32_t num)
{
    for (int i = sizeof(num) * CHAR_BIT - 1; i >= 0; i--)
    {
        std::cout << (((num & (1 << i)) == 0) ? "0" : "1");
    }
    std::cout << std::endl;
}

int main(void)
{
    int32_t inserted{-5};
    uint8_t buffer[4]{0};

    buffer_insert(buffer, 0, 4, inserted);
    int32_t extracted{static_cast<int32_t>(buffer_extract(buffer, 0, 4))};

    print(inserted);
    print(extracted);

    return 0;
}

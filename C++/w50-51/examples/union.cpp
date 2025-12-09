#include <iostream>

union Data
{
    static uint64_t s;
    uint32_t dword;
    uint16_t word[2];
    uint8_t byte[4];
};

uint64_t Data::s; // Initialized by 0

int main(void)
{
    Data data{0x12345678};

    std::cout << data.s << std::endl;
    std::cout << Data::s << std::endl;
    std::cout << sizeof(Data) << std::endl;

    std::cout << "0x" << std::hex << data.dword << std::endl;

    std::cout << "0x" << std::hex << data.dword << std::endl;

    std::cout << "0x" << std::hex << data.word[0] << std::endl;

    std::cout << "0x" << std::hex << static_cast<int>(data.byte[3]) << std::endl;

    return 0;
}
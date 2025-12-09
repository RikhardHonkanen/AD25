#include <iostream>

enum struct Color : std::uint8_t
{
    RED,
    GREEN = 6,
    BLUE,
    UNKNOWN
};

int main(void)
{
    enum Status
    {
        ERROR,
        WARNING,
        OKAY,
        UNKNOWN = OKAY + 100
    };

    Status status{OKAY};
    status = Status::WARNING;
    std::cout << status << std::endl;

    Color color{Color::GREEN};
    std::cout << static_cast<int>(Color::BLUE) << std::endl;
    std::cout << ((color == Color::BLUE) ? "Blue" : "Green") << std::endl;

    return 0;
}

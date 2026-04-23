#include <thread>
#include <iostream>

// To compile the code run: g++ -O1 14-volatile-shared.cpp -o main && ./main

#if 0 // Change 1 to 0 and look at the ouput.
static bool status{false};
#else
volatile static bool status{false};
#endif

void func(void)
{
    while (!status)
    {
        ;
    }

    std::cout << "Out of the loop" << std::endl;
}

int main(void)
{
    std::thread thrd{func};

    std::this_thread::sleep_for(std::chrono::seconds(2));
    status = true;

    thrd.join();

    return 0;
}
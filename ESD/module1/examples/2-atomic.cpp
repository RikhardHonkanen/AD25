#include <thread>
#include <atomic>
#include <iostream>

static std::atomic<int> count{0};

void increment(void)
{
    for (int i = 0; i < 10000; i++)
    {
        count++; // The operation is atomic
    }
}

int main(void)
{
    std::thread t1{increment};
    std::thread t2{increment};
    std::thread t3{increment};
    std::thread t4{increment};
    std::thread t5{increment};

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    std::cout << "Count value:" << count << std::endl;

    return 0;
}
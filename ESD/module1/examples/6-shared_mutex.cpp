/*
In contrast to other mutex types which facilitate exclusive access, a shared_mutex has two levels of access:
    shared - several threads can share ownership of the same mutex.
    exclusive - only one thread can own the mutex.

    https://en.cppreference.com/w/cpp/thread/shared_mutex
*/

#include <mutex>
#include <thread>
#include <iostream>
#include <shared_mutex>

class Counter
{
    uint32_t value = 0;
    mutable std::shared_mutex mtx;

public:
    Counter() = default;

    // Multiple threads/readers can read the counter's value at the same time.
    uint32_t get() const
    {
        mtx.lock_shared();
        uint32_t val{value};
        mtx.unlock_shared();

        return val;
    }

    // Only one thread/writer can increment/write the counter's value.
    void increment()
    {
        mtx.lock();
        ++value;
        mtx.unlock();
    }

    // Only one thread/writer can reset/write the counter's value.
    void reset()
    {
        mtx.lock();
        value = 0;
        mtx.unlock();
    }
};

int main(void)
{
    Counter counter;

    auto increment_and_print = [&counter]()
    {
        static std::mutex iomtx;

        for (int i = 0; i < 3; i++)
        {
            counter.increment();

            {
                iomtx.lock();
                std::cout << std::this_thread::get_id() << ' ' << counter.get() << std::endl;
                iomtx.unlock();
            }
        }
    };

    std::thread thread1(increment_and_print);
    std::thread thread2(increment_and_print);

    thread1.join();
    thread2.join();

    return 0;
}
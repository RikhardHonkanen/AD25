/*
    The class shared_lock is a general-purpose shared mutex ownership wrapper allowing deferred locking,
        timed locking and transfer of lock ownership. Locking a shared_lock locks the associated shared mutex
        in shared mode (to lock it in exclusive mode, std::unique_lock can be used).
        The shared_lock class is movable, but not copyable.
        Look at: https://en.cppreference.com/w/cpp/thread/shared_lock
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
        std::shared_lock<std::shared_mutex> lock(mtx);
        return value;
    }

    // Only one thread/writer can increment/write the counter's value.
    void increment()
    {
        std::unique_lock<std::shared_mutex> lock(mtx);
        ++value;
    }

    // Only one thread/writer can reset/write the counter's value.
    void reset()
    {
        std::unique_lock<std::shared_mutex> lock(mtx);
        value = 0;
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
                std::scoped_lock<std::mutex> lock{iomtx};
                std::cout << std::this_thread::get_id() << ' ' << counter.get() << std::endl;
            }
        }
    };

    std::thread thread1(increment_and_print);
    std::thread thread2(increment_and_print);

    thread1.join();
    thread2.join();

    return 0;
}
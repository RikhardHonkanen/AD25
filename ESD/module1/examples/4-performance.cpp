#include <mutex>
#include <chrono>
#include <vector>
#include <thread>
#include <iostream>

std::mutex mutex;
std::vector<uint32_t> vec;
constexpr uint32_t THREADS_NUM{4};
constexpr uint32_t RANGE_MAX{100000};

void find_primes(const uint32_t start, const uint32_t end)
{
    // Cycle through numbers while ignoring evens
    for (uint32_t x = start; x <= end; x += 2)
    {
        // If a modulus is 0 we know it isn't prime
        for (uint32_t y = 2; y < x; y++)
        {
            if ((x % y) == 0)
            {
                break;
            }
            else if ((y + 1) == x)
            {
                mutex.lock();
                vec.push_back(x);
                mutex.unlock();
            }
        }
    }
}

void find_primes_with_thread(const uint32_t start, const uint32_t end, const uint32_t threads_num)
{
    std::vector<std::thread> tvec;

    // Divide up the calculation so each thread
    // operates on different primes
    uint32_t new_start = start;
    uint32_t thread_spread = end / threads_num;
    uint32_t new_end = start + thread_spread - 1;

    // Create prime list for each thread
    for (uint32_t x = 0; x < threads_num; x++)
    {
        tvec.emplace_back(find_primes, new_start, new_end);

        new_start += thread_spread;
        new_end += thread_spread;
    }

    for (auto &thrd : tvec)
    {
        thrd.join();
    }
}

int main(void)
{
    // Get time before code starts executing
    auto start = std::chrono::system_clock::now();

    // Find primes without threads
    find_primes(1, RANGE_MAX);

    // Get the execution time
    std::chrono::duration<double, std::micro> passed = std::chrono::system_clock::now() - start;

    // Print out the number of microseconds
    std::cout << "Execution Time : " << passed.count() << " us" << std::endl;

    vec.clear();

    // Get time before code starts executing
    start = std::chrono::system_clock::now();

    find_primes_with_thread(1, RANGE_MAX, THREADS_NUM); // Find primes with THREADS_NUM threads

    // Get the execution time
    passed = std::chrono::system_clock::now() - start;

    // Print out the number of microseconds
    std::cout << "Execution Time : " << passed.count() << " us" << std::endl;

    return 0;
}
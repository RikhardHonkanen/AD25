/**
 * @file exercise-2.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief  Using a mutex, a condition variable and 3 threads every secode print 1 - 2 - 3 to the terminal.
 *         The first thread shall print 1 - , the second thread shall print 2 -
 *         and the third thread shall print 3 and newline.
 *         The threads shall be synchronized.
 *
 * @version 0.1
 * @date 2021-06-21
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <mutex>
#include <chrono>
#include <thread>
#include <iostream>
#include <condition_variable>

constexpr int MIN{1};
constexpr int MAX{3};
static std::mutex mtx;
static volatile int number{1};
static std::condition_variable cv;

static void print_num(const int num)
{
    while (true)
    {
        if (num == MIN)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        std::unique_lock<std::mutex> lck{mtx};

        cv.wait(lck, [num]
                { return (number == num); });

        if (number == MAX)
        {
            std::cout << number << std::endl;
            number = MIN;
        }
        else
        {
            std::cout << number << " - ";
            number++;
        }

        lck.unlock();
        cv.notify_all();
    }
}

int main(void)
{
    std::thread threads[MAX];

    for (int i = 1; i <= MAX; i++)
    {
        threads[i - 1] = std::thread{print_num, i};
    }

    for (std::thread &thrd : threads)
    {
        thrd.join();
    }

    return 0;
}

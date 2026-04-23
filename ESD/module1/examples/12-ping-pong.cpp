/**
 * @file ping-pong.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief  Using two threads make a program to print “Ping - Pong”
 *         10 times to the terminal. A thread shall print Ping and the other thread
 *         shall Pong to the terminal. Using a condition variable ensure the right order so that the output
 *         looks like:
 *            Ping - Pong
 *            Ping - Pong
 *            Ping - Pong
 *            ...
 *
 * @version 0.1
 * @date 2021-06-21
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <mutex>
#include <thread>
#include <iostream>
#include <condition_variable>

constexpr int PING{0};
constexpr int PONG{1};
constexpr int ITRATIONS{10};

static void print_ping(void); // The ping thread function
static void print_pong(void); // The pong thread function

static std::mutex mtx;
static std::condition_variable cv;
static volatile int ping_pong = PING; // The shared variable

int main(void)
{
    // Create the threads
    std::thread ping_thread{print_ping};
    std::thread pong_thread{print_pong};

    // Wait on the threads to get completed
    ping_thread.join();
    pong_thread.join();

    return 0;
}

static void print_ping(void)
{
    for (int i = 0; i < ITRATIONS; i++)
    {
        std::unique_lock<std::mutex> lock{mtx};

        cv.wait(lock, []
                { return (ping_pong == PING); });

        std::cout << "Ping - ";
        ping_pong = PONG;

        lock.unlock();   // Unlock the mutex before notifying to avoid waking up
        cv.notify_one(); // Notify the pong thread
    }
}

static void print_pong(void)
{
    for (int i = 0; i < ITRATIONS; i++)
    {
        std::unique_lock<std::mutex> lock{mtx};

        cv.wait(lock, []
                { return (ping_pong == PONG); });

        std::cout << "Pong" << std::endl;
        ping_pong = PING;

        lock.unlock();
        cv.notify_one();
    }
}

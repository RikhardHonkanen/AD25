/**
 * @file ping-pong.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief  Using two threads make a program to print “Ping - Pong”
 *         10 times to the terminal. A thread shall print Ping and the other thread
 *         shall Pong to the terminal. Ensure the right order so that the output
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

constexpr int PING{0};
constexpr int PONG{1};
constexpr int ITRATIONS{10};

static std::mutex mtx;
static volatile int ping_pong{PING}; // The shared resource

// The ping thread function
// Synchronized using an emulated condition variable
static void print_ping(void)
{
    for (int i = 0; i < ITRATIONS; i++)
    {
        mtx.lock(); // Lock the mutex
        while (ping_pong != PING)
        {
            mtx.unlock();              // Unlock the mutex
            std::this_thread::yield(); // Reschedule the execution of the thread, allowing the other thread to run.
            mtx.lock();                // Lock the mutex
        }

        std::cout << "Ping - ";
        ping_pong = PONG; // Change the shared variable

        mtx.unlock();              // Unlock the mutex
        std::this_thread::yield(); // Reschedule the execution of the thread, allowing the other thread to run.
    }
}

// The pong thread function
// Synchronized using an emulated condition variable
static void print_pong(void)
{
    for (int i = 0; i < ITRATIONS; i++)
    {
        mtx.lock(); // Lock the mutex
        while (ping_pong != PONG)
        {
            mtx.unlock();              // Unlock the mutex
            std::this_thread::yield(); // Reschedule the execution of the thread, allowing the other thread to run.
            mtx.lock();                // Lock the mutex
        }

        std::cout << "Pong" << std::endl;
        ping_pong = PING; // Change the shared variable

        mtx.unlock();              // Unlock the mutex
        std::this_thread::yield(); // Reschedule the execution of the thread, allowing the other thread to run.
    }
}

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

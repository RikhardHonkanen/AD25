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
static void print_ping(void)
{
    for (int i = 0; i < ITRATIONS; i++)
    {
        // Wait until ping_pong is not PING
        // Note that this method of synchronization is not optimized and
        // the CPU is executing an instruction during the waiting time
        // and we can say that the CUP power is wasted.
        while (ping_pong != PING)
        {
            ;
        }

        mtx.lock(); // Lock the mutex
        std::cout << "Ping - ";
        ping_pong = PONG; // Change the shared variable
        mtx.unlock();     // Unlock the mutex
    }
}

// The pong thread function
static void print_pong(void)
{
    for (int i = 0; i < ITRATIONS; i++)
    {
        // Wait until ping_pong is not PONG
        // Note that this method of synchronization is not optimized and
        // the CPU is executing an instruction during the waiting time
        // and we can say that the CUP power is wasted.
        while (ping_pong != PONG)
        {
            ;
        }

        mtx.lock(); // Lock the mutex
        std::cout << "Pong" << std::endl;
        ping_pong = PING; // Change the shared variable
        mtx.unlock();     // Unlock the mutex
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

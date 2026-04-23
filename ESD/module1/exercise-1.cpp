/**
 * @file exercise-1.cpp
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief  Using two threads make a program to print “Ping - Pong” every second to the terminal.
 *         A thread shall print Ping and the other thread shall Pong to the terminal.
 *         Using two mutexes ensure the right order so that the output looks like:
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

static void print_ping(void); // The ping thread function
static void print_pong(void); // The pong thread function

static std::mutex ping, pong; // Two mutexes to manage the order

int main(void)
{
    pong.lock();

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
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ping.lock();
        std::cout << "Ping - ";
        pong.unlock();
    }
}

static void print_pong(void)
{
    while (1)
    {
        pong.lock();
        std::cout << "Pong" << std::endl;
        ping.unlock();
    }
}

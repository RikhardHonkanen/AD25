/**
 * @file ping-pong.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief  Using two threads make a program to print “Ping - Pong”
 *         every second to the terminal. A thread shall print Ping and the other thread
 *         shall Pong to the terminal. Ensure the right order so that the output
 *         looks like:
 *            Ping - Pong
 *            Ping - Pong
 *            Ping - Pong
 *            ...
 * @version 0.1
 * @date 2021-06-21
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

#define PING 0
#define PONG 1

static volatile uint8_t ping_pong = PING;                 // The shared resource
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Create and initialize a mutex to protect the shared resource

// The ping thread function
static void *print_ping(void *args)
{
    (void)args; // Unused arguments shall be discarded by casting them to void

    while (1)
    {
        sleep(1);

        // Wait until ping_pong is not PING
        // Note that this method of synchronization is not optimized and
        // the CPU is executing some instructions during the waiting time
        // and we can say that the CUP power is wasting.
        while (ping_pong != PING)
        {
            ;
        }

        pthread_mutex_lock(&mutex); // Lock the mutex
        printf("Ping - ");
        ping_pong = PONG;             // Change the shared variable
        pthread_mutex_unlock(&mutex); // Unlock the mutex
    }
}

// The pong thread function
static void *print_pong(void *args)
{
    (void)args; // args is discarded

    while (1)
    {
        // Wait until ping_pong is not PONG
        // Note that this method of synchronization is not optimized and
        // the CPU is executing some instructions during the waiting time
        // and we can say that the CUP power is wasting.
        while (ping_pong != PONG)
        {
            ;
        }

        pthread_mutex_lock(&mutex); // Lock the mutex
        printf("Pong\n");
        ping_pong = PING;             // Change the shared variable
        pthread_mutex_unlock(&mutex); // Unlock the mutex
    }
}

int main(void)
{
    pthread_t ping_thread, pong_thread;

    // Create and check the ping thread
    if (0 != pthread_create(&ping_thread, NULL, print_ping, NULL))
    {
        printf("Failed to create ping_thread!\n");
        exit(EXIT_FAILURE);
    }

    // Create and check the pong thread
    if (0 != pthread_create(&pong_thread, NULL, print_pong, NULL))
    {
        printf("Failed to create pong_thread!\n");
        exit(EXIT_FAILURE);
    }

    // Wait on the threads to get completed
    pthread_join(ping_thread, NULL);
    pthread_join(pong_thread, NULL);

    // Release the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}

/**
 * @file ping-pong.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief  Using two threads make a program to print “Ping - Pong”
 *         every second to the terminal. A thread shall print Ping and the other thread
 *         shall Pong to the terminal. Using a condition variable ensure the right order so that the output
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
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#define PING 0
#define PONG 1

static volatile uint8_t ping_pong = PING;                   // The shared variable
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;   // The mutex to control access to the shared variable
static pthread_cond_t condition = PTHREAD_COND_INITIALIZER; // The condition variable used to synchronize the threads

// The ping thread function
static void *print_ping(void *arg)
{
    (void)arg;

    while (1)
    {
        sleep(1);

        pthread_mutex_lock(&mutex); // Lock the mutex

        while (ping_pong != PING)
        {
            pthread_cond_wait(&condition, &mutex); // Wait until the condtion variable gets signaled
        }

        printf("Ping - ");
        ping_pong = PONG;

        pthread_cond_signal(&condition); // Signal the waiting thread (pong thread).
        pthread_mutex_unlock(&mutex);    // Release the mutex
    }
}

// The pong thread function
static void *print_pong(void *arg)
{
    (void)arg;

    while (1)
    {
        pthread_mutex_lock(&mutex); // Lock the mutex

        while (ping_pong != PONG)
        {
            pthread_cond_wait(&condition, &mutex); // Wait until the condtion variable gets signaled
        }

        printf("Pong\n");
        ping_pong = PING;

        pthread_cond_signal(&condition); // Signal the waiting thread (ping thread).
        pthread_mutex_unlock(&mutex);    // Release the mutex
    }
}

int main(void)
{
    pthread_t ping_thread, pong_thread;

    // Create and check the ping thread
    if (0 != pthread_create(&ping_thread, NULL, print_ping, NULL))
    {
        printf("Faild to create ping_thread!\n");
        exit(EXIT_FAILURE);
    }

    // Create and check the pong thread
    if (0 != pthread_create(&pong_thread, NULL, print_pong, NULL))
    {
        printf("Faild to create pong_thread!\n");
        exit(EXIT_FAILURE);
    }

    // Wait on the threads to get completed
    pthread_join(ping_thread, NULL);
    pthread_join(pong_thread, NULL);

    // Release the resources
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);

    return 0;
}
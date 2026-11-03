/**
 * @file ping.cpp
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief An example of a POSIX IPC Named Semaphore.
 *        To build: g++ ping.cpp -lpthread -lrt -o ping.
 *        Named semaphores are mounted under /dev/shm: ls /dev/shm
 *
 * @version 0.1
 * @date 2024-11-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>

constexpr int ITERATIONS{10};
const char *const SEM_PING{"/ping"};
const char *const SEM_PONG{"/pong"};

int main(void)
{
    (void)sem_unlink(SEM_PING);
    (void)sem_unlink(SEM_PONG);

    sem_t *sem_ping = sem_open(SEM_PING, O_CREAT | O_RDWR, 0666, 1);
    sem_t *sem_pong = sem_open(SEM_PONG, O_CREAT | O_RDWR, 0666, 0);

    if ((sem_ping == SEM_FAILED) || (sem_pong == SEM_FAILED))
    {
        std::cout << "Failed to create the semaphores." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    else
    {
        for (int i = 0; i < ITERATIONS; i++)
        {
            (void)sem_wait(sem_ping);
            std::cout << "Ping" << std::endl;
            sleep(1);
            (void)sem_post(sem_pong);
        }

        (void)sem_destroy(sem_ping);
        (void)sem_destroy(sem_pong);

        (void)sem_unlink(SEM_PING);
        (void)sem_unlink(SEM_PONG);
    }

    return 0;
}

/**
 * @file unnamed-shared-memory.cpp
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief An example of a POSIX IPC Unnamed Semaphore.
 *        Look at: https://man7.org/linux/man-pages/man7/sem_overview.7.html
 *        To build: g++ main.cpp -lpthread -lrt -o main
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

constexpr int PROCESSES_NUM{5};

int main(void)
{
    // The semaphore object shall be located in a region of shared memory
    sem_t *semaphore = (sem_t *)mmap(nullptr, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    if (semaphore == MAP_FAILED)
    {
        std::cout << "Failed to create the semaphore." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if (-1 == sem_init(semaphore, 1, 1))
    {
        (void)sem_destroy(semaphore);
        std::cout << "Failed to initialize the semaphore" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Fork multiple child processes
    for (int i = 0; i < PROCESSES_NUM; i++)
    {
        pid_t pid = fork();

        if (pid == -1)
        {
            std::cout << "Failed to fork a child process." << std::endl;
            std::exit(EXIT_FAILURE);
        }
        else if (pid == 0) /* Child process */
        {
            (void)sem_wait(semaphore);

            std::cout << "Child " << i << " has acquired the semaphore!"
                      << std::endl
                      << std::flush;

            sleep(2); // Simulate a work

            (void)sem_post(semaphore);

            std::exit(EXIT_SUCCESS);
        }
    }

    // Wait for child processes to finish
    for (int i = 0; i < PROCESSES_NUM; i++)
    {
        (void)wait(nullptr);
    }

    (void)sem_destroy(semaphore); // Destroy the semaphore

    (void)munmap(semaphore, sizeof(sem_t)); // Unmap the shared memory

    return 0;
}

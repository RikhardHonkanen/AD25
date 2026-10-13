/**
 * @file named-shared-memory.cpp
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief An example of POSIX IPC Shared Memory.
 *        Look at: https://man7.org/linux/man-pages/man7/shm_overview.7.html
 *        To build: g++ main.cpp -lrt -o main
 *        While the program is sleep run: cat /dev/shm/myshm
 *
 * @version 0.1
 * @date 2024-11-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/mman.h>

constexpr int SHM_SIZE{1024};
const char *const SHM_NAME{"/myshm"};

int main(void)
{
    // Create a shared memory object
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        std::cout << "Failed to create a shared memory object" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Configure the size of the shared memory segment
    if (-1 == ftruncate(shm_fd, SHM_SIZE))
    {
        std::cout << "Failed to set size of shared memory object" << std::endl;
        close(shm_fd);
        shm_unlink(SHM_NAME);
        std::exit(EXIT_FAILURE);
    }

    // Map the shared memory segment into the address space of this process
    char *shmp = (char *)mmap(nullptr, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shmp == MAP_FAILED)
    {
        std::cout << "Failed to map the shared memory segment" << std::endl;
        close(shm_fd);
        shm_unlink(SHM_NAME);
        std::exit(EXIT_FAILURE);
    }

    // Write a message to the shared memory
    std::strcpy(shmp, "Hello, Shared Memory!");
    std::cout << "Message written to shared memory: " << shmp << std::endl;

    pid_t pid = fork(); // Fork a child process

    if (pid == -1)
    {
        std::cout << "Failed to fork the child process" << std::endl;
    }
    else if (pid == 0) /* Child process reads from shared memory */
    {
        std::cout << "Child process reads from shared memory: " << shmp << std::endl;
    }

    // Sleep a while
    sleep(10);

    // Unmap and close the shared memory
    munmap(shmp, SHM_SIZE);
    close(shm_fd);

    // Remove the shared memory object
    (void)shm_unlink(SHM_NAME);

    return 0;
}

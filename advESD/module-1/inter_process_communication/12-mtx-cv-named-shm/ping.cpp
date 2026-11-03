/**
 * @file ping.cpp
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief An example of POSIX mutex, condition variable and shared memory
 *        To build: g++ ping.cpp -lpthread -lrt -o ping.
 *        Note: run ping before pong.
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
#include <pthread.h>
#include <sys/mman.h>
#include <semaphore.h>

struct ping_pong_t
{
    int value;
    pthread_cond_t cv;
    pthread_mutex_t mtx;
};

constexpr int ITERATIONS{10};
constexpr int PING{0}, PONG{1};
const char *const SHM_NAME{"/ping_pong"};

int main(void)
{
    shm_unlink(SHM_NAME);
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        std::cout << "Failed to create a shared memory" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if (-1 == ftruncate(shm_fd, sizeof(ping_pong_t)))
    {
        std::cout << "Failed to set size of shared memory object" << std::endl;
        close(shm_fd);
        shm_unlink(SHM_NAME);
        std::exit(EXIT_FAILURE);
    }

    ping_pong_t *ping_pong = (ping_pong_t *)mmap(nullptr, sizeof(ping_pong_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ping_pong == MAP_FAILED)
    {
        std::cout << "Failed to map the shared memory segment" << std::endl;
        close(shm_fd);
        shm_unlink(SHM_NAME);
        std::exit(EXIT_FAILURE);
    }

    ping_pong->value = PING;

    pthread_mutexattr_t mtxattr;
    pthread_mutexattr_init(&mtxattr);
    pthread_mutexattr_setpshared(&mtxattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&ping_pong->mtx, &mtxattr);

    pthread_condattr_t cvattr;
    pthread_condattr_init(&cvattr);
    pthread_condattr_setpshared(&cvattr, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(&ping_pong->cv, &cvattr);

    for (int i = 0; i < ITERATIONS; i++)
    {
        pthread_mutex_lock(&ping_pong->mtx);
        while (ping_pong->value != PING)
        {
            pthread_cond_wait(&ping_pong->cv, &ping_pong->mtx);
        }

        std::cout << "Ping" << std::endl;
        ping_pong->value = PONG;

        pthread_cond_signal(&ping_pong->cv);
        pthread_mutex_unlock(&ping_pong->mtx);
    }

    pthread_mutex_lock(&ping_pong->mtx);
    while (ping_pong->value != PING)
    {
        pthread_cond_wait(&ping_pong->cv, &ping_pong->mtx);
    }
    pthread_mutex_unlock(&ping_pong->mtx);

    pthread_mutex_destroy(&ping_pong->mtx);
    pthread_cond_destroy(&ping_pong->cv);
    pthread_mutexattr_destroy(&mtxattr);
    pthread_condattr_destroy(&cvattr);

    (void)munmap(ping_pong, sizeof(ping_pong_t));
    (void)close(shm_fd);
    (void)shm_unlink(SHM_NAME);

    return 0;
}

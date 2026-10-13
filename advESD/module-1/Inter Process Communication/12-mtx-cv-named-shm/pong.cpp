/**
 * @file pong.cpp
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief An example of POSIX mutex, condition variable and shared memory
 *        To build: g++ pong.cpp -lpthread -lrt -o pong.
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
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1)
    {
        std::cout << "Failed to open the shared memory object" << std::endl;
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

    for (int i = 0; i < ITERATIONS; i++)
    {
        pthread_mutex_lock(&ping_pong->mtx);
        while (ping_pong->value != PONG)
        {
            pthread_cond_wait(&ping_pong->cv, &ping_pong->mtx);
        }

        std::cout << "Pong" << std::endl;
        ping_pong->value = PING;
        sleep(1);

        pthread_cond_signal(&ping_pong->cv);
        pthread_mutex_unlock(&ping_pong->mtx);
    }

    (void)munmap(ping_pong, sizeof(ping_pong_t));
    (void)close(shm_fd);

    return 0;
}

/**
 * @file reader.cpp
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief An example of POSIX IPC Message Queues
 *        Look at: https://man7.org/linux/man-pages/man7/mq_overview.7.html
 *        To build: g++ receiver.cpp -lrt -o receiver, To show: cat /dev/mqueue/mymq
 *
 * @version 0.1
 * @date 2024-11-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <cstring>
#include <mqueue.h>
#include <iostream>

constexpr int MAX_MSG_SIZE{1024};
const char *const QUEUE_NAME{"/mymq"};

int main(void)
{
    // Open the message queue for reading
    mqd_t mq = mq_open(QUEUE_NAME, O_RDONLY);
    if (mq == -1)
    {
        std::cout << "Failed to open the queue" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    char buffer[MAX_MSG_SIZE];

    while (1)
    {
        std::memset(buffer, 0, sizeof(buffer));

        if (-1 == mq_receive(mq, buffer, MAX_MSG_SIZE, nullptr))
        {
            std::cout << "Failed to receive a message" << std::endl;
            break;
        }

        std::cout << "Received message: " << buffer << std::endl;

        if (0 == std::strcmp(buffer, "exit"))
        {
            break;
        }
    }

    mq_close(mq); // Close the message queue

    return 0;
}

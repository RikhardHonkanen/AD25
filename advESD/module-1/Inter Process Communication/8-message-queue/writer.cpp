/**
 * @file writer.cpp
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief An example of POSIX IPC Message Queues.
 *        Look at: https://man7.org/linux/man-pages/man7/mq_overview.7.html
 *        To build: g++ sender.cpp -lrt -o sender, To show: cat /dev/mqueue/mymq
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
    mq_attr attr{0}; // Set up message queue attributes
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_SIZE;

    // Open the message queue
    mqd_t mq = mq_open(QUEUE_NAME, O_WRONLY | O_CREAT, 0666, &attr);
    if (mq == -1)
    {
        std::cout << "Failed to open the queue" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    char buffer[MAX_MSG_SIZE];

    while (1)
    {
        std::memset(buffer, 0, sizeof(buffer));

        std::cout << "Enter a message: ";
        std::cin >> buffer;

        // Send the message
        if (-1 == mq_send(mq, buffer, strlen(buffer) + 1, 0))
        {
            std::cout << "Failed to send the message" << std::endl;
            break;
        }
        if (0 == std::strcmp(buffer, "exit"))
        {
            break;
        }
    }

    mq_close(mq);          // Close the message queue
    mq_unlink(QUEUE_NAME); // Remove the message queue

    return 0;
}

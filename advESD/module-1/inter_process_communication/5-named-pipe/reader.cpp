/**
 * @file reader.cpp
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief A named pipe(FIFO file) example.
 * @version 0.1
 * @date 2024-11-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <ctime>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <sys/stat.h>

constexpr int MAX_LEN{32};
const char *const FIFO_NAME{"/tmp/myfifo"}; // FIFO file path

int main(void)
{
    srand(time(nullptr));

    (void)mkfifo(FIFO_NAME, 0666); // Create the FIFO if it doesn't exist

    int fd;
    char buffer[MAX_LEN + 1] = {0};

    while (1)
    {
        fd = open(FIFO_NAME, O_RDONLY); // First open it in read only and read

        if (fd == -1)
        {
            std::cout << "Failed to open the FIFO..." << std::endl;
            break;
        }
        if (sizeof(buffer) != read(fd, buffer, sizeof(buffer)))
        {
            std::cout << "Failed to read from the FIFO..." << std::endl;
            close(fd);
            break;
        }

        close(fd);

        std::cout << "Received:" << buffer << std::endl;

        for (int i = 0; i < sizeof(buffer) - 1; i++)
        {
            buffer[i] = 'a' + (rand() % 26);
        }

        fd = open(FIFO_NAME, O_WRONLY); // Open it in write mode and write
        if (fd == -1)
        {
            std::cout << "Failed to open the FIFO..." << std::endl;
            break;
        }

        if (sizeof(buffer) != write(fd, buffer, sizeof(buffer)))
        {
            std::cout << "Failed to write to the FIFO..." << std::endl;
            close(fd);
            break;
        }

        close(fd);
    }

    return 0;
}

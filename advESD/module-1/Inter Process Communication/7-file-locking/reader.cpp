/**
 * @file reader.cpp
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief An example of file locking mechanism
 * @version 0.1
 * @date 2024-11-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int main(void)
{
    const char *file_path = "lockfile";

    // Open the file for reading
    int fd = open(file_path, O_RDONLY);
    if (fd == -1)
    {
        std::cout << "Failed to open the file." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Lock the file for reading
    flock lock;
    lock.l_type = F_RDLCK; // Read lock
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLKW, &lock) == -1)
    {
        std::cout << "Failed to lock the file." << std::endl;
        close(fd);
        std::exit(EXIT_FAILURE);
    }

    char buffer[64] = {0};

    // Read data from the locked file
    if (0 < read(fd, buffer, sizeof(buffer) - 1))
    {
        std::cout << "Read from locked file: " << buffer << std::endl;
    }
    else
    {
        std::cout << "Failed to read from the file." << std::endl;
    }

    lock.l_type = F_UNLCK; // Unlock the file

    if (-1 == fcntl(fd, F_SETLK, &lock))
    {
        std::cout << "Failed to unlock the file." << std::endl;
    }

    close(fd); // Close the file

    return 0;
}

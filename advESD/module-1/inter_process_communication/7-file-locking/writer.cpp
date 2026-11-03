/**
 * @file writer.cpp
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief An example of file locking mechanism
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

int main(void)
{
    const char *file_path = "lockfile";
    const char *message = "This is a locked file example.";

    // Open the file for writing
    int fd = open(file_path, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        std::cout << "Failed to open the file." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Attempt to lock the file
    flock lock;
    lock.l_type = F_WRLCK; // Write lock
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    if (-1 == fcntl(fd, F_SETLKW, &lock))
    {
        std::cout << "Failed to lock the file." << std::endl;
        close(fd);
        std::exit(EXIT_FAILURE);
    }

    // Write data to the locked file
    if (-1 == write(fd, message, strlen(message)))
    {
        std::cout << "Failed to write to the file." << std::endl;
        close(fd);
        std::exit(EXIT_FAILURE);
    }

    sleep(10); // Sleep 10s while the file is locked

    lock.l_type = F_UNLCK; // Unlock the file

    if (-1 == fcntl(fd, F_SETLKW, &lock))
    {
        std::cout << "Failed to unlock the file." << std::endl;
    }

    close(fd); // Close the file

    return 0;
}

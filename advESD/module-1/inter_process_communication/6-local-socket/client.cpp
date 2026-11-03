/**
 * @file client.cpp
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief A local or UNIX-domain socket client example
 * @version 0.1
 * @date 2024-11-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <sys/un.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>

constexpr int MAX_LEN{32};
const char *const SOCKET_NAME{"/tmp/local_socket"};

int main(void)
{
    std::srand(std::time(nullptr));

    int client_socket = socket(AF_LOCAL, SOCK_STREAM, 0); // Create a socket
    if (client_socket == -1)
    {
        std::cout << "Failed to create the socket..." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    sockaddr_un servaddr{0}; // Set up the address structure
    servaddr.sun_family = AF_LOCAL;
    strncpy(servaddr.sun_path, SOCKET_NAME, sizeof(servaddr.sun_path) - 1);

    if (0 != connect(client_socket, (sockaddr *)&servaddr, sizeof(servaddr)))
    {
        std::cout << "Connection to the server failed..." << std::endl;
        close(client_socket);
        std::exit(EXIT_FAILURE);
    }

    while (1)
    {
        char buffer[MAX_LEN + 1]{0};

        int len = 1 + std::rand() % MAX_LEN;

        for (int i = 0; i < len; i++)
        {
            buffer[i] = 'a' + std::rand() % 26;
        }

        std::cout << "Sent :" << buffer << std::endl;

        if (len != write(client_socket, buffer, len))
        {
            std::cout << "Failed to write ..." << std::endl;
            break;
        }

        bzero(buffer, sizeof(buffer));

        if (len != read(client_socket, buffer, len))
        {
            std::cout << "Failed to read ..." << std::endl;
            break;
        }

        std::cout << "Received:" << buffer << std::endl
                  << std::endl;

        sleep(1);
    }

    close(client_socket); // Close the socket

    return 0;
}

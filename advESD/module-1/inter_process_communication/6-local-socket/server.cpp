/**
 * @file server.cpp
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief A local or UNIX-domain socket server example
 * @version 0.1
 * @date 2024-11-27
 *
 * @copyright Copyright (c) 2024
 *
 */
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
    unlink(SOCKET_NAME); // Remove the socket file if it exists

    int server_socket = socket(AF_LOCAL, SOCK_STREAM, 0); // Create a socket

    if (server_socket == -1)
    {
        std::cout << "Failed to create the socket..." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    sockaddr_un servaddr{0}; // Set up the address structure
    servaddr.sun_family = AF_LOCAL;
    strncpy(servaddr.sun_path, SOCKET_NAME, sizeof(servaddr.sun_path) - 1);

    // Bind the socket to the specified address
    if (0 != bind(server_socket, (sockaddr *)&servaddr, sizeof(servaddr)))
    {
        std::cout << "Failed to bind to the socket..." << std::endl;
        close(server_socket);
        std::exit(EXIT_FAILURE);
    }

    if (0 != listen(server_socket, 1)) /* only one connection */
    {
        std::cout << "Failed to listen to the port..." << std::endl;
        close(server_socket);
        std::exit(EXIT_FAILURE);
    }

    std::cout << "Server is waiting for connections..." << std::endl;

    // Accept a connection
    int client_socket = accept(server_socket, nullptr, nullptr);
    if (client_socket == -1)
    {
        std::cout << "Failed to accept the connection..." << std::endl;
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "Server acccepted the client..." << std::endl;
    }

    while (1)
    {
        char buffer[MAX_LEN + 1]{0};

        // Read the message from client and store it in buffer
        if (1 > read(client_socket, buffer, MAX_LEN))
        {
            std::cout << "Failed to read ..." << std::endl;
            close(client_socket);
            break;
        }

        size_t len = strlen(buffer);

        for (size_t i = 0; i < len; i++)
        {
            buffer[i] = toupper(buffer[i]);
        }

        // Send buffer to client
        if (len != write(client_socket, buffer, len))
        {
            std::cout << "Failed to write ..." << std::endl;
            close(client_socket);
            break;
        }
    }

    close(server_socket);

    unlink(SOCKET_NAME); // Remove the socket file

    return 0;
}

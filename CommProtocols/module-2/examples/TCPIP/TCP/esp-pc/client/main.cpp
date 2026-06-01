#include <ctime>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

constexpr int MAX_LEN{32};
constexpr int PORT{12345};
const char *SERVER{"192.168.4.1"};

int main(void)
{
    // Create socket and check
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sockfd == -1)
    {
        std::cout << "Failed to create the socket..." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    sockaddr_in servaddr{0};

    // Assign IP and PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(SERVER);

    // Connect to the server
    if (0 == connect(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)))
    {
        std::cout << "Connected to the server."
                  << std::endl
                  << std::endl;

        std::srand(std::time(nullptr));

        while (1)
        {
            char buffer[MAX_LEN + 1]{0};
            int len = 1 + std::rand() % MAX_LEN;

            for (int i = 0; i < len; i++)
            {
                buffer[i] = 'a' + std::rand() % 26;
            }
            std::cout << "Sent    : " << buffer << std::endl;

            // Send buffer to the server
            if (len != write(sockfd, buffer, len))
            {
                std::cout << "Failed to write ..." << std::endl;
                break;
            }

            bzero(buffer, sizeof(buffer));

            // Receive data from the server and store it in buffer
            if (len != read(sockfd, buffer, len))
            {
                std::cout << "Failed to read ..." << std::endl;
                break;
            }

            std::cout << "Received: " << buffer
                      << std::endl
                      << std::endl;

            sleep(1);
        }
    }
    else
    {
        std::cout << "Connection to the server failed." << std::endl;
    }

    // Close the socket
    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);

    return EXIT_FAILURE;
}
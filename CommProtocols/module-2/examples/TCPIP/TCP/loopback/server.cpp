#include <cctype>
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Binding newly created socket to given IP and verification
    if (0 == bind(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)))
    {
        if (0 == listen(sockfd, 1)) // only one connection
        {
            sockaddr_in cli{0};
            socklen_t len = sizeof(cli);

            // Accept and check the connection
            int connfd = accept(sockfd, (sockaddr *)&cli, &len);
            if (connfd >= 0)
            {
                std::cout << "Server acccepted the client..." << std::endl;

                while (1)
                {
                    char buffer[MAX_LEN + 1]{0};

                    // Read the message from client and store it in buffer
                    if (1 > read(connfd, buffer, MAX_LEN))
                    {
                        std::cout << "Failed to read ..." << std::endl;
                        break;
                    }

                    size_t len = strlen(buffer);

                    for (size_t i = 0; i < len; i++)
                    {
                        buffer[i] = toupper(buffer[i]);
                    }

                    // Send buffer to client
                    if (len != write(connfd, buffer, len))
                    {
                        std::cout << "Failed to write ..." << std::endl;
                        break;
                    }
                }

                shutdown(connfd, SHUT_RDWR);
                close(connfd); // Close the connection
            }
            else
            {
                std::cout << "Failed to accept the connection..." << std::endl;
            }
        }
        else
        {
            std::cout << "Failed to listen to the port..." << std::endl;
        }
    }
    else
    {
        std::cout << "Failed to bind servaddr to the socket..." << std::endl;
    }

    // Close the socket
    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);

    return EXIT_FAILURE;
}

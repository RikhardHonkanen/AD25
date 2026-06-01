#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLEN 32U
#define SERVER_PORT 12345U

int main(void)
{
    // Create UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sockfd == -1)
    {
        printf("Failed to create a socket\n");
    }
    else
    {
        struct sockaddr_in server_addr = {0};

        // Configure server address
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(SERVER_PORT);
        server_addr.sin_addr.s_addr = INADDR_ANY;

        // Bind the socket to the server address
        if (-1 == bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)))
        {
            printf("Failed to bind\n");
        }
        else
        {
            printf("UDP server is running on port %d...\n", SERVER_PORT);

            socklen_t client_len;
            char buffer[MAXLEN + 1];
            struct sockaddr_in client_addr = {0};

            while (1)
            {
                memset(buffer, 0, sizeof(buffer));

                client_len = sizeof(struct sockaddr_in);

                // Receive data from clients
                if (-1 == recvfrom(sockfd, buffer, MAXLEN, 0, (struct sockaddr *)&client_addr, &client_len))
                {
                    printf("Failed to receive\n");
                    break;
                }
                printf("Received message from %s:%d - %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

                size_t buflen = strlen(buffer);
                for (size_t i = 0; i < buflen; i++)
                {
                    buffer[i] = toupper(buffer[i]);
                }

                // Send to the client
                if (-1 == sendto(sockfd, buffer, buflen, 0, (struct sockaddr *)&client_addr, client_len))
                {
                    printf("Failed to send\n");
                    break;
                }
            }

            // Close the socket
            shutdown(sockfd, SHUT_RDWR);
            close(sockfd);
        }
    }

    return EXIT_FAILURE;
}

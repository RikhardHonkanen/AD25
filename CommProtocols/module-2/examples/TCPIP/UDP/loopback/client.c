#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAXLEN 32U
#define PORT 12345U

int main(void)
{
    // Creating a socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sockfd < 0)
    {
        printf("Socket creation failed\n");
    }
    else
    {
        char buffer[MAXLEN + 1] = {0};
        struct sockaddr_in servaddr = {0};

        // Filling server information
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        servaddr.sin_addr.s_addr = INADDR_ANY;

        srand(time(NULL));

        while (1)
        {
            memset(buffer, 0, sizeof(buffer));
            int buflen = 1 + rand() % MAXLEN;

            for (int i = 0; i < buflen; i++)
            {
                buffer[i] = 'a' + (rand() % 26);
            }
            printf("    Sent: %s\n", buffer);

            // Look at: https://linux.die.net/man/2/sendto
            if (-1 == sendto(sockfd, buffer, buflen, 0, (const struct sockaddr *)&servaddr, sizeof(servaddr)))
            {
                printf("Failed to send\n");
                break;
            }

            memset(buffer, 0, sizeof(buffer));

            // Look at: https://linux.die.net/man/2/recvfrom
            if (-1 == recvfrom(sockfd, buffer, MAXLEN, 0, (struct sockaddr *)&servaddr, (socklen_t *)&buflen))
            {
                printf("Failed to receive\n");
                break;
            }

            printf("Received: %s\n\n", buffer);

            sleep(1);
        }

        shutdown(sockfd, SHUT_RDWR);
        close(sockfd);
    }

    return EXIT_FAILURE;
}
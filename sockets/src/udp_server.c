#include "../include/server_util.h"

int main()
{
    int sockfd = sock_s(1), f;
    char buffer[1024], *codeword;

    struct sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));

    while (1)
    {
        printf("waiting for client\n");
        recv_udp_s(sockfd, buffer, &cliaddr);

        f = 0;

        strcpy(buffer, "Choose the error detection method:\n");
        strcat(buffer, "1. Checksum\n");
        strcat(buffer, "2. VRC\n");
        strcat(buffer, "3. LRC\n");
        strcat(buffer, "4. CRC\n");
        strcat(buffer, "5. Exit\n");

        send_udp_s(sockfd, buffer, &cliaddr);
        recv_udp_s(sockfd, buffer, &cliaddr);

        switch (*buffer)
        {
        case '1':
            strcpy(buffer, "Enter the data word: ");
            send_udp_s(sockfd, buffer, &cliaddr);
            recv_udp_s(sockfd, buffer, &cliaddr);

            sprintf(buffer, "Checksum: %d\r\n", checksum(buffer));
            send_udp_s(sockfd, buffer, &cliaddr);

            break;
        case '2':
            strcpy(buffer, "Enter the data word: ");
            send_udp_s(sockfd, buffer, &cliaddr);
            recv_udp_s(sockfd, buffer, &cliaddr);

            codeword = getVRC(buffer);
            sprintf(buffer, "Code Word: %s\r\n", codeword);
            send_udp_s(sockfd, buffer, &cliaddr);
            free(codeword);
            break;
        case '3':
            strcpy(buffer, "Enter the data word: ");
            send_udp_s(sockfd, buffer, &cliaddr);
            recv_udp_s(sockfd, buffer, &cliaddr);

            codeword = getLRC(buffer);
            sprintf(buffer, "Code Word: %s\r\n", codeword);
            send_udp_s(sockfd, buffer, &cliaddr);
            free(codeword);
            break;
        case '4':
            strcpy(buffer, "Enter the data word: ");
            send_udp_s(sockfd, buffer, &cliaddr);
            recv_udp_s(sockfd, buffer, &cliaddr);

            codeword = getCRC(buffer);
            sprintf(buffer, "Code Word: %s\r\n", codeword);
            send_udp_s(sockfd, buffer, &cliaddr);
            // free(codeword);
            break;
        default:
            f = 1;
        }
        if (f || *buffer == '5')
        {
            break;
        }
    }
    close(sockfd);
    return 0;
}

#include "../include/server_util.h"

int main()
{
    int sockfd = sock_s(0), f;
    char buffer[1024], *codeword;

    while (1)
    {
        int connfd;
        if ((connfd = conn_s(sockfd)) == -1)
            continue;
        printf("connection success\n");

        while (1)
        {
            f = 0;

            strcpy(buffer, "Choose the error detection method:\n");
            strcat(buffer, "1. Checksum\n");
            strcat(buffer, "2. VRC\n");
            strcat(buffer, "3. LRC\n");
            strcat(buffer, "4. CRC\n");
            strcat(buffer, "5. Exit\n");

            send_tcp_s(connfd, buffer);
            recv_tcp_s(connfd, buffer);

            switch (*buffer)
            {
            case '1':
                strcpy(buffer, "Enter the data word: ");
                send_tcp_s(connfd, buffer);
                recv_tcp_s(connfd, buffer);

                sprintf(buffer, "Checksum: %d\r\n", checksum(buffer)); // The carriage return (\r) is often used in conjunction with newline (\n) in network communications and file formats, particularly those designed to be compatible with a variety of operating systems.
                send_tcp_s(connfd, buffer);

                break;
            case '2':
                strcpy(buffer, "Enter the data word: ");
                send_tcp_s(connfd, buffer);
                recv_tcp_s(connfd, buffer);

                codeword = getVRC(buffer);
                sprintf(buffer, "Code Word: %s\r\n", codeword);
                send_tcp_s(connfd, buffer);
                free(codeword);
                break;
            case '3':
                strcpy(buffer, "Enter the data word: ");
                send_tcp_s(connfd, buffer);
                recv_tcp_s(connfd, buffer);

                codeword = getLRC(buffer);
                sprintf(buffer, "Code Word: %s\r\n", codeword);
                send_tcp_s(connfd, buffer);
                free(codeword);
                break;
            case '4':
                strcpy(buffer, "Enter the data word: ");
                send_tcp_s(connfd, buffer);
                recv_tcp_s(connfd, buffer);

                codeword = getCRC(buffer);
                sprintf(buffer, "Code Word: %s\r\n", codeword);
                send_tcp_s(connfd, buffer);
                free(codeword);
                break;
            default:
                f = 1;
            }
            if (f || *buffer == '5')
            {
                close(connfd);
                break;
            }
        }
    }
    close(sockfd); // Not necessary as the server's gonna be shutdown using ctrl+c interrupt always which automatically cleans up memory resources
    return 0;
}

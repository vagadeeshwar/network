#include "../include/client_util.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n", argv[0]);
        return 1;
    }

    int sockfd = conn_c(argv[1], 0);
    char buffer[1024];

    while (1)
    {
        recv_tcp_c(sockfd, buffer);
        scanf("%s", buffer); // scanf reads until whitespace so \n is not included in buffer... buffer is automatically null terminated
        send_tcp_c(sockfd, buffer);

        recv_tcp_c(sockfd, buffer);
        scanf("%s", buffer);
        send_tcp_c(sockfd, buffer);

        recv_tcp_c(sockfd, buffer);
        printf("\n");
    }
    close(sockfd);
    return 0;
}

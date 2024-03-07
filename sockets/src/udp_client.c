#include "../include/client_util.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n", argv[0]);
        return 1;
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    int sockfd = conn_c(argv[1], 1);
    char buffer[1024];

    while (1)
    {
        send_udp_c(sockfd, buffer, &servaddr); // empty request just to start the conversation... as client has to send req first and only then serv can respond in udp

        recv_udp_c(sockfd, buffer, &servaddr);
        scanf("%s", buffer); // scanf reads until whitespace so \n is not included in buffer... buffer is automatically null terminated
        send_udp_c(sockfd, buffer, &servaddr);

        recv_udp_c(sockfd, buffer, &servaddr);
        scanf("%s", buffer);
        send_udp_c(sockfd, buffer, &servaddr);

        recv_udp_c(sockfd, buffer, &servaddr);
        printf("\n");
    }
    close(sockfd);
    return 0;
}

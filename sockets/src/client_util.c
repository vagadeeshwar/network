#include "../include/server_util.h"

#define PORT_TCP 5007
#define PORT_UDP 10005

void send_udp_c(int sockfd, char *buffer, struct sockaddr_in *servptr)
{
    if (sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)servptr, sizeof(*servptr)) == -1)
        perror("Client - Error sending data\n");
}

void recv_udp_c(int sockfd, char *buffer, struct sockaddr_in *servptr)
{
    char c;
    int i;
    socklen_t len = sizeof(*servptr);

    for (i = 0; recvfrom(sockfd, &c, 1, 0, (struct sockaddr *)servptr, &len) > 0 && c != 0; i++) // socket still active && \0 not reached
        buffer[i] = c;
    buffer[i] = 0;

    if (i == -1)
        perror("Client - Read error\n");
    else if (i == 0)
    {
        exit(EXIT_SUCCESS);
        close(sockfd);
    }
    else
        printf("%s", buffer);
}

void send_tcp_c(int sockfd, char *buffer)
{
    if (write(sockfd, buffer, strlen(buffer) + 1) == -1)
        perror("Client - Error sending data\n");
}

void recv_tcp_c(int sockfd, char *buffer)
{
    char c;
    int i;

    for (i = 0; read(sockfd, &c, 1) > 0 && c != 0; i++) // socket still active && \0 not reached
        buffer[i] = c;
    buffer[i] = 0;

    if (i == -1)
        perror("Client - Read error\n");
    else if (i == 0)
    {
        exit(EXIT_SUCCESS);
        close(sockfd);
    }
    else
        printf("%s", buffer);
}

int conn_c(char *ip, int type) // type=0 for tcp and anyother value for udp
{
    int sockfd, e;
    struct sockaddr_in serv_addr;
    if (!type)
    {
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("Client - can't create socket\n");
            exit(EXIT_FAILURE);
        }
    }
    else if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Client - can't create socket\n");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    if (!type)
        serv_addr.sin_port = htons(PORT_TCP);
    else
        serv_addr.sin_port = htons(PORT_UDP);

    if ((e = inet_pton(AF_INET, ip, &serv_addr.sin_addr)) == 0) // checks if entered address is a valid ipv4 address
    {
        perror("Client - Invalid network address was entered\n");
        exit(EXIT_FAILURE);
    }
    else if (e == -1)
    {
        perror("Client - Network address conversion failed\n");
        exit(EXIT_FAILURE);
    }
    if (!type)
        if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        {
            perror("Client - Connecting to server failed\n");
            exit(EXIT_FAILURE);
        }

    return sockfd;
}

/*sockfd is a file descriptor that points to a socket. In a client application, sockfd is used to connect to the server and to send/receive data from the server.

connfd is another file descriptor that points to a socket, but it is typically used in server applications. When a client connects to a server, the server's accept function creates a new socket for that specific client-server communication, and connfd is the file descriptor for that socket. This way, the server can keep its original socket (sockfd) open to accept more client connections, while using the new socket (connfd) to communicate with the specific client.

Here's a basic summary:

In a client program: you use sockfd to connect to a server and to send/receive data.

In a server program: you use sockfd to bind/listen/accept connections, and use connfd (the result of an accept call) to send/receive data with the connected client.

In your client code, you should be using sockfd to send/receive data since sockfd is the socket you used to connect to the server. There is no connfd in your client code because your client isn't accepting connections; it's making a connection to a server.*/

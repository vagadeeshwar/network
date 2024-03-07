#include "../include/server_util.h"

#define PORT_TCP 5007
#define PORT_UDP 10005
#define BACKLOG 10    // maximum number of connections the kernel should queue for this socket.
#define POLY "1101\0" // Generator polynomial

void send_udp_s(int sockfd, char *buffer, struct sockaddr_in *cliptr)
{
    if (sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)cliptr, sizeof(*cliptr)) == -1)
        perror("Server - Error sending data\n");
}

void recv_udp_s(int sockfd, char *buffer, struct sockaddr_in *cliptr)
{
    char c;
    int i;
    socklen_t len = sizeof(*cliptr);

    for (i = 0; recvfrom(sockfd, &c, 1, 0, (struct sockaddr *)cliptr, &len) > 0 && c != 0; i++) // socket still active && eol not reached
        buffer[i] = c;
    buffer[i] = 0;

    if (i == -1)
        perror("Server - Read error\n");
    else
        printf("Data read success: %s\n", buffer);
}

void send_tcp_s(int connfd, char *buffer)
{
    if (write(connfd, buffer, strlen(buffer) + 1) == -1) // strlen doesn't count \0 so length + 1 should be written into buffer so that client can read properly
        perror("Server - Error sending data\n");
}

void recv_tcp_s(int connfd, char *buffer)
{
    char c;
    int i;

    for (i = 0; read(connfd, &c, 1) > 0 && c != 0; i++) // socket still active && eol not reached
        buffer[i] = c;
    buffer[i] = 0;

    if (i == -1)
        perror("Server - Read error\n");
    else
        printf("Data read success: %s\n", buffer);
}

int sock_s(int type) // type=0 for tcp and anyother value for udp
{
    struct sockaddr_in serv_addr; // struct sockaddr_in is the datatype and serv_addr is an instance we are creating
    int sockfd;

    if (!type)
    {
        // Creating socket
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("Server - can't create socket\n");
            exit(EXIT_FAILURE);
        }
    }
    else if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Server - can't create socket\n");
        exit(EXIT_FAILURE);
    }

    // sets all bytes in the memory area pointed to by &serv_addr to the ASCII value of '0'
    memset(&serv_addr, '0', sizeof(serv_addr)); // However, with structures that are passed to system calls, such as struct sockaddr_in, not all members of the structure are always set explicitly.For example, the struct sockaddr_in contains a field sin_zero which is used to pad the structure to the length of a struct sockaddr.This field should be set to all zeros according to the specification, but it's often ignored in code, relying on the memset call to zero it out.

    serv_addr.sin_family = AF_INET; // AF_INET -> Ipv4 Addresses
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (!type)
        serv_addr.sin_port = htons(PORT_TCP);
    else
        serv_addr.sin_port = htons(PORT_UDP);

    // Binding the socket
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Server - bind failed\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (!type)
    {
        // Setting up listening with a queue of 10 connections
        if (listen(sockfd, 10) == -1)
        {
            perror("Server - listen failed\n");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        printf("Listening\n");
    }
    return sockfd;
}

int conn_s(int sockfd)
{
    int connfd;

    printf("Trying to accept a connection\n");
    // Accepting a connection
    if ((connfd = accept(sockfd, (struct sockaddr *)NULL, NULL)) == -1)
    {
        perror("Server - accept failed\n");
        return -1;
    }
    return connfd;
}
unsigned int checksum(const char *str)
{
    unsigned int sum = 0;
    while (*str)
    {
        sum += (unsigned int)(*str);
        str++;
    }
    return sum;
}

char *getVRC(const char *input)
{
    int length = strlen(input);
    char *codeword = (char *)malloc((length + 2) * sizeof(char));

    int count = 0;
    for (int i = 0; i < length; i++)
    {
        codeword[i] = input[i];
        if (input[i] == '1')
            count++;
    }

    if (count % 2 == 0)
        codeword[length] = '0';
    else
        codeword[length] = '1';
    codeword[length + 1] = '\0';

    return codeword;
}

char *getLRC(const char *input)
{
    int length = strlen(input);
    int paddedLength = ((length + 7) / 8) * 8; // Round up to the next multiple of 8

    // Construction of the padded dataword
    char *padded = (char *)malloc(paddedLength + 1);
    memset(padded, '0', paddedLength - length);    // Pad with zeroes
    strcpy(padded + paddedLength - length, input); // Copy the original string
    padded[paddedLength] = '\0';

    length = strlen(padded);
    int blocks = length / 8;

    // Allocate memory for the LRC (8 bits + 1 for null character)
    char *lrc = (char *)malloc(9);
    lrc[8] = '\0';

    // Calculate the LRC
    for (int i = 0; i < 8; i++)
    {
        int count = 0;
        for (int j = 0; j < blocks; j++)
            if (padded[j * 8 + i] == '1')
                count++;
        lrc[i] = (count % 2 == 0) ? '0' : '1';
    }

    free(padded); // Free the allocated memory for the padded string
    return lrc;
}

// Perform XOR operation
void xor (char *a, char *b, int len) {
    for (int j = 1; j < len; j++)
        a[j] = ((a[j] == b[j]) ? '0' : '1');
}

    // Calculate CRC
    char *getCRC(const char *input)
{
    int lenData = strlen(input), i, j, lenPoly = strlen(POLY);
    char *check_value = malloc(sizeof(char) * (lenPoly + 1));
    int totalLen = lenData + lenPoly - 1;

    printf("genpoly length: %d\n", lenPoly);

    // Append lenPoly-1 zeros to data
    char *dividend = (char *)malloc(totalLen + 1);
    strcpy(dividend, input);

    for (i = lenData; i < totalLen; i++)
        dividend[i] = '0';
    dividend[totalLen] = '\0';

    printf("Padded Dataword: %s\n", dividend);

    for (i = 0; i < lenPoly; i++)
        check_value[i] = dividend[i];
    check_value[i] = '\0';
    printf("check value length: %lu\n", strlen(check_value));

    do
    {
        // check if the first bit is 1 and calls XOR function
        if (check_value[0] == '1')
            xor(check_value, POLY, lenPoly);
        // Move the bits by 1 position for the next computation
        for (int j = 0; j < lenPoly - 1; j++)
            check_value[j] = check_value[j + 1];

        // appending a bit from data
        check_value[j] = dividend[i++];

        printf("Iterationx - check_value: %s\n", check_value);
    } while (i <= lenData + lenPoly - 1);
    // loop until the data ends

    check_value[lenPoly - 1] = '\0';
    free(dividend);
    return check_value;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 5000
#define BUFSIZE 1024

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n", argv[0]);
        return 1;
    }

    int n, sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    char buf[BUFSIZE], *usr = malloc(sizeof(char) * 20), *pwd = malloc(sizeof(char) * 20);
    struct sockaddr_in servaddr;
    socklen_t len;

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);

    printf("Enter your username:");
    scanf("%s", usr);
    printf("Enter your password:");
    scanf("%s", pwd);

    sendto(sockfd, (const char *)usr, strlen(usr), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    sendto(sockfd, (const char *)pwd, strlen(pwd), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    n = recvfrom(sockfd, (char *)buf, BUFSIZE, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
    buf[n] = '\0';

    if (*buf == '0')
        printf("Your access to the website has been authorized!\n");
    else if (*buf == '1')
        printf("Enter correct username!\n");
    else
        printf("Enter correct password!\n");

    close(sockfd);
    free(usr);
    free(pwd);

    return 0;
}

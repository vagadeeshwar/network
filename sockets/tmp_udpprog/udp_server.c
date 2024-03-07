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

int main()
{
    int n, sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    char buf[BUFSIZE], str[100];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    char *usr_list[10] = {
        "user1", "user2", "user3", "user4", "user5",
        "user6", "user7", "user8", "user9", "user10"},
         *pwd_list[10] = {"pwd1", "pwd2", "pwd3", "pwd4", "pwd5", "pwd6", "pwd7", "pwd8", "pwd9", "pwd10"};

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    while (1)
    {
        len = sizeof(cliaddr);

        n = recvfrom(sockfd, (char *)buf, BUFSIZE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buf[n] = '\0';
        strcpy(str, buf);

        printf("\n Connection From %s, Port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, buf, sizeof(buf)), ntohs(cliaddr.sin_port));
        printf("Received username: %s\n", str);

        n = recvfrom(sockfd, (char *)buf, BUFSIZE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buf[n] = '\0';
        printf("Received password: %s\n", buf);

        n = -1;
        for (int i = 0; i < 10; i++)
        {
            if (!strcmp(*(usr_list + i), str))
            {
                n = i;
                break;
            }
            if (i == 9)
            {
                *buf = '1';
                *(buf + 1) = 0;
                sendto(sockfd, (const char *)buf, strlen(buf), 0, (const struct sockaddr *)&cliaddr, len);
            }
        }
        if (n != -1 && !strcmp(*(pwd_list + n), buf))
        {
            *buf = '0';
            *(buf + 1) = 0;
            sendto(sockfd, (const char *)buf, strlen(buf), 0, (const struct sockaddr *)&cliaddr, len);
            continue;
        }

        *buf = '2';
        *(buf + 1) = 0;
        sendto(sockfd, (const char *)buf, strlen(buf), 0, (const struct sockaddr *)&cliaddr, len);
    }

    return 0;
}

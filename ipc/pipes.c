#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_BUF_SIZE 1024

long fact(int n)
{
    if (n < 0)
    {
        printf("Factorial of a negative number doesn't exist.\n");
        exit(EXIT_FAILURE);
    }
    return (n == 0 || n == 1) ? 1 : fact(n - 1) * n;
}

int main()
{
    int pipefds1[2], pipefds2[2];
    pid_t pid;
    char readmessage[MAX_BUF_SIZE], writemessage[MAX_BUF_SIZE];

    if (pipe(pipefds1) == -1) // parent writes and child reads
    {
        printf("Unable to create pipe 1 \n");
        return 1;
    }

    if (pipe(pipefds2) == -1) // child writes and parent reads
    {
        printf("Unable to create pipe 2 \n");
        return 1;
    }

    pid = fork();
    if (pid != 0) // Parent process
    {
        close(pipefds1[0]); // Close the unwanted pipe1 read side
        close(pipefds2[1]); // Close the unwanted pipe2 write side

        printf("In Parent: Enter the number to calculate factorial: ");
        fgets(writemessage, MAX_BUF_SIZE, stdin);

        write(pipefds1[1], writemessage, sizeof(writemessage));

        read(pipefds2[0], readmessage, sizeof(readmessage));
        printf("In Parent: The calculated factorial is %s\n", readmessage);
    }
    else
    {
        close(pipefds1[1]); // Close the unwanted pipe1 write side
        close(pipefds2[0]); // Close the unwanted pipe2 read side

        read(pipefds1[0], readmessage, sizeof(readmessage));

        sprintf(writemessage, "%ld", fact(atoi(readmessage))); // sprintf does int to str conversion
        write(pipefds2[1], writemessage, sizeof(writemessage));
    }
    return 0;
}

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define FIFO_FILE1 "/tmp/namedpipe_serverread"
#define FIFO_FILE2 "/tmp/namedpipe_serverwrite"

int main()
{
    int fd1, fd2, len;
    char readbuf[80];

    printf("FIFO_CLIENT: Send messages, infinitely, to end enter \"q\"\n");

    fd1 = open(FIFO_FILE1, O_WRONLY);
    fd2 = open(FIFO_FILE2, O_RDONLY);

    while (1)
    {
        printf("Enter string: ");
        fgets(readbuf, sizeof(readbuf), stdin);
        readbuf[strlen(readbuf) - 1] = '\0'; // fgets does include a newline character (\n) at the end of the string if a newline is read, and it always null-terminates (\0) the string.
        len = strlen(readbuf);

        if (strcmp(readbuf, "q") != 0)
        {
            write(fd1, readbuf, len);
            printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n",
                   readbuf, len);


            read(fd2, readbuf, sizeof(readbuf));
            printf("FIFOCLIENT: Received string: \"%s\" and string length is %d\n",
                   readbuf, len);
        }
        else
        {
            write(fd1, readbuf, len);
            printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n",
                   readbuf, len);
            close(fd1);
            close(fd2);
            break;
        }
    }
    return 0;
}

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define FIFO_FILE1 "/tmp/namedpipe_serverread"  // files inside /tmp are automatically cleared before every boot
#define FIFO_FILE2 "/tmp/namedpipe_serverwrite" // temporary files created at the root of file structure inside tmp folder to facilitate namedpipe comm

void reverse_string(char *);

int main()
{
    int fd1, fd2;
    char readbuf[80];
    int read_bytes;

    // Create two FIFOs for bidirectional communication
    mkfifo(FIFO_FILE1, S_IFIFO | 0640); // S_IFIFO is a bitmask used to check if a file is a FIFO special file (i.e., a named pipe) in POSIX compliant operating systems.
    mkfifo(FIFO_FILE2, S_IFIFO | 0640);

    fd1 = open(FIFO_FILE1, O_RDONLY);
    fd2 = open(FIFO_FILE2, O_WRONLY);

    while (1)
    {
        read_bytes = read(fd1, readbuf, sizeof(readbuf) - 1);
        readbuf[read_bytes] = '\0'; // strlen reads until \0 is encountered
        printf("FIFOSERVER: Received string: \"%s\" and length is %d\n", readbuf, read_bytes);

        if (strcmp(readbuf, "q") == 0)
        {
            close(fd1);
            close(fd2);
            break;
        }

        reverse_string(readbuf);
        printf("FIFOSERVER: Sending Reversed String: \"%s\" and length is %d\n", readbuf, read_bytes);
        write(fd2, readbuf, read_bytes + 1);
        sleep(2); // 2 seconds
    }
    return 0;
}

void reverse_string(char *str)
{
    int last, limit, first;
    char temp;
    last = strlen(str) - 1;
    limit = last / 2;
    first = 0;

    while (first < last)
    {
        temp = str[first];
        str[first] = str[last];
        str[last] = temp;
        first++;
        last--;
    }
    return;
}

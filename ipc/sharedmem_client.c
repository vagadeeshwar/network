#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>

#define SHM_KEY 12345

int main()
{
    // Get shared memory segment
    int shmid = shmget(SHM_KEY, 1024, 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        perror("Failed to create/get shared memory segment");
        return 1;
    }

    // Attach to the segment to get a pointer to it
    char *str = (char *)shmat(shmid, (void *)0, 0);

    // Get a string from user and write it to shared memory
    printf("Enter a string: ");
    fgets(str, 1024, stdin);
    str[strcspn(str, "\n")] = '\0'; // Remove trailing newline

    // Wait for the server to write the result into shared memory
    while (strcmp(str, "true") != 0 && strcmp(str, "false") != 0)
        ;

    if (strcmp(str, "true") == 0)
        printf("The string is a palindrome.\n");
    else
        printf("The string is not a palindrome.\n");

    // Detach from the shared memory segment
    shmdt(str);

    return 0;
}

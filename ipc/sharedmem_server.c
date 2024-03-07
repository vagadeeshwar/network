// Gives correct output only when the client input is written to shared memory first and then the server is started
//  This is due to race conditions and absence of synchronization

#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define SHM_KEY 12345

bool is_palindrome(char *);

int main()
{
    // Get shared memory segment
    int shmid = shmget(SHM_KEY, 1024, 0666 | IPC_CREAT); // The IPC_CREAT flag ensures that the shared memory segment is created if it does not already exist, and 0666 is the permission bits that grant read and write access to all users.
    if (shmid == -1)
    {
        perror("Failed to create/get shared memory segment");
        return 1;
    }

    // Attach to the segment to get a pointer to it
    char *str = (char *)shmat(shmid, (void *)0, 0);

    // Wait for a string to be written into shared memory
    while (str[0] == '\0')
        ;

    if (is_palindrome(str))
        strcpy(str, "true");
    else
        strcpy(str, "false");

    // Detach from the shared memory segment
    shmdt(str);

    // Destroy the shared memory segment
    int ret;
    do // shmctl call with the IPC_RMID command is typically used to mark a shared memory segment to be destroyed. The actual destruction will only happen after the last process detaches it
    {
        if ((ret = shmctl(shmid, IPC_RMID, NULL)) == -1)
        {
            perror("shmctl - Error deleting shared memory segment"); // some other process still using the shared segment
            sleep(1);                                                // Wait for a bit before retrying
        }
    } while (ret == -1);

    return 0;
}

bool is_palindrome(char *str)
{
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++)
        if (str[i] != str[len - i - 1])
            return false;
    return true;
}
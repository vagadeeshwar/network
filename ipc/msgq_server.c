#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_MSG_SIZE 100
#define MSG_KEY 1234

struct message
{
    long msg_type;
    int number;
    int is_prime;
};

int is_prime(int);

int main()
{
    int msg_id;
    struct message msg;

    // access message queue
    if ((msg_id = msgget(MSG_KEY, 0666 | IPC_CREAT)) == -1) // The IPC_CREAT flag ensures that the message queue is created if it does not already exist, and 0666 is the permission bits that grant read and write access to all users.
    {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    // receive message
    if (msgrcv(msg_id, &msg, sizeof(msg), 1, 0) == -1)
    {
        perror("msgrcv failed");
        exit(EXIT_FAILURE);
    }

    printf("Received number: %d\n", msg.number);
    msg.is_prime = is_prime(msg.number);

    msg.msg_type = 2; // Send messages of type 2
    if (msgsnd(msg_id, &msg, sizeof(msg), 0) == -1)
    {
        perror("msgsnd failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int is_prime(int n)
{
    if (n <= 1)
        return 0;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0)
            return 0;
    return 1;
}
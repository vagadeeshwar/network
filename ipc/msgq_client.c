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

int main()
{
    int msg_id;
    struct message msg;

    // create message queue
    msg_id = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (msg_id == -1)
    {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    printf("Enter an integer: ");
    scanf("%d", &(msg.number));

    // send message
    msg.msg_type = 1;
    if (msgsnd(msg_id, &msg, sizeof(msg), 0) == -1)
    {
        perror("msgsnd failed");
        exit(EXIT_FAILURE);
    }

    // receive message
    if (msgrcv(msg_id, &msg, sizeof(msg), 2, 0) == -1)
    {
        perror("msgrcv failed");
        exit(EXIT_FAILURE);
    }

    printf("Is prime: %s\n", msg.is_prime ? "Yes" : "No");

    // destroy message queue
    if (msgctl(msg_id, IPC_RMID, NULL) == -1) // msgq destroy implemented in client instead of server as : In your server code, you are calling msgctl to remove the message queue right after receiving and responding to the client's message. However, the client may still be waiting to receive this response when the queue is removed, which causes the error.
    {
        perror("msgctl failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}

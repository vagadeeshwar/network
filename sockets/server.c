#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    char buffer[BUFFER_SIZE] = {0};
    char *message = "Hello from server!";

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening
    if (listen(server_socket, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    socklen_t addrlen = sizeof(client_address);
    printf("Waiting for connections ...\n");

    // Accept a connection
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &addrlen)) < 0)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected with client\n");

    // Read message from client
    read(client_socket, buffer, BUFFER_SIZE);
    printf("Message from client: %s\n", buffer);

    // Send message to the client
    write(client_socket, message, strlen(message));
    printf("Message sent to client\n");

    return 0;
}

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define SA struct sockaddr

#define PORT 8888
#define BUFFER_SIZE 1024

int clientSocket;

void* read_msg(void* arg) {
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            // Server disconnected
            continue;
        }

        printf("\nReceived: %s\n", buffer);
    }

    pthread_exit(NULL);
}

void send_msg() {
    char buffer[BUFFER_SIZE];

    while (1) {
        printf("Enter message: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "exit") == 0) {
            // Exit the loop and disconnect from server
            printf("Disconnecting from server.\n");
            break;
        }

        if (send(clientSocket, buffer, strlen(buffer), 0) < 0) {
            perror("Failed to send message");
            break;
        }
    }

    // Close client socket
    close(clientSocket);
}

int main() {
	int connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1) {
		printf("socket creation failed...\n");
		exit(0);
	} else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);


    // Connect to the server
    if (connect(clientSocket, (SA*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Failed to connect to server");
        exit(1);
    }

    // Create thread for reading messages
    pthread_t thread;
    if (pthread_create(&thread, NULL, read_msg, NULL) != 0) {
        perror("Failed to create read_msg thread");
        exit(1);
    }

    // Send messages from the main thread
    send_msg();

    // Wait for the read_msg thread to exit
    pthread_join(thread, NULL);

    return 0;
}

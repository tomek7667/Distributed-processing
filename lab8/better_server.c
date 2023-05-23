#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define EPOLL_SIZE 10

int clientSockets[MAX_CLIENTS];
pthread_t threads[MAX_CLIENTS];
int numClients = 0;

void* clientHandler(void* arg) {
    int clientSocket = *((int*)arg);
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesRead <= 0) {
            // Client disconnected
            break;
        }

        // Broadcast message to all clients
        for (int i = 0; i < numClients; i++) {
            if (clientSockets[i] != clientSocket) {
                // send(clientSockets[i], buffer, bytesRead, 0);
                // don't do it like this! Send via FD
            }
        }
    }

    // Close client socket
    close(clientSocket);

    // Remove client from the list
    for (int i = 0; i < numClients; i++) {
        if (clientSockets[i] == clientSocket) {
            for (int j = i; j < numClients - 1; j++) {
                clientSockets[j] = clientSockets[j + 1];
                threads[j] = threads[j + 1];
            }
            numClients--;
            break;
        }
    }

    pthread_exit(NULL);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    int epoll_fd;

    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Failed to create server socket");
        exit(1);
    }

    // Set server address
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8888);

    // Bind server socket to the specified address
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Failed to bind server socket");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(serverSocket, MAX_CLIENTS) < 0) {
        perror("Failed to listen for connections");
        exit(1);
    }

    // Create epoll instance
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("Failed to create epoll instance");
        exit(1);
    }

    // Add server socket to epoll event list
    struct epoll_event serverEvent;
    serverEvent.events = EPOLLIN;
    serverEvent.data.fd = serverSocket;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serverSocket, &serverEvent) == -1) {
        perror("Failed to add server socket to epoll");
        exit(1);
    }

    printf("Server started, waiting for connections...\n");

    while (1) {
        struct epoll_event events[EPOLL_SIZE];
        int numEvents = epoll_wait(epoll_fd, events, EPOLL_SIZE, -1);
        if (numEvents == -1) {
            perror("Failed to wait for events");
            exit(1);
        }
        for (int i = 0; i < numEvents; i++) {
            if (events[i].data.fd == serverSocket) {
                // New client connection
                clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
                if (clientSocket < 0) {
                    perror("Failed to accept client connection");
                    continue;
                }

                printf("New client connected: %d\n", clientSocket);

                // Add client socket to epoll event list
                struct epoll_event clientEvent;
                clientEvent.events = EPOLLIN | EPOLLRDHUP;
                clientEvent.data.fd = clientSocket;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientSocket, &clientEvent) == -1) {
                    perror("Failed to add client socket to epoll");
                    exit(1);
                }

                // Create a new thread to handle the client
                if (pthread_create(&threads[numClients], NULL, clientHandler, &clientSocket) != 0) {
                    perror("Failed to create client handler thread");
                    exit(1);
                }

                // Add client socket to the list
                clientSockets[numClients] = clientSocket;
                numClients++;
            } else {
                // Data received from a client
                int clientSocket = events[i].data.fd;
                pthread_t thread = threads[i];

                if (events[i].events & EPOLLIN) {
                    // Handle client message in the respective thread
                    if (pthread_join(thread, NULL) != 0) {
                        perror("Failed to join client handler thread");
                        exit(1);
                    }

                    if (pthread_create(&thread, NULL, clientHandler, &clientSocket) != 0) {
                        perror("Failed to create client handler thread");
                        exit(1);
                    }

                    threads[i] = thread;
                } else if (events[i].events & EPOLLRDHUP) {
                    // Client disconnected
                    pthread_cancel(thread);
                }
            }
        }
    }

    // Close server socket
    close(serverSocket);

    return 0;
}

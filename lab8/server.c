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
#define MAX 80
#define MAX_BUFFER_SIZE 1024
#define PORT 8080
#define SA struct sockaddr

typedef struct message message;

struct message{
	char message[MAX_BUFFER_SIZE];
};

// array of thread ids
int clients[5];
int num_clients = 0;
// BEGIN LAB 8

// array of pipes
int pipes[5][2];

// END LAB 8

void* func(void* arg) {
    int client_i = num_clients;
    int connfd = *(int*)arg;
    // BEING LAB 8
    int fd[2];
    pipe(fd);
    int epoll_fd = epoll_create1(0);
    struct epoll_event events[MAX];
    struct epoll_event event1;
    // EPOLLIN - reading
    event1.events = EPOLLIN;
    event1.data.fd = fd[0];
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd[0], &event1);
    struct epoll_event event2;
    event2.events = EPOLLIN;
    // socket client fd
    event2.data.fd = connfd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connfd, &event2);

    clients[client_i] = connfd; // adding client to list
    pipes[client_i][1] = fd[1];
    pipes[client_i][0] = fd[0];
    printf("New client appeared: %d\n", client_i);
    client_i++;

    // END LAB 8

    char buff[MAX];
    int n;

    write(fd[1], "Welcome to the chatroom!\n", MAX_BUFFER_SIZE);

    for (;;) {
        bzero(buff, MAX);
        // read(connfd, buff, sizeof(buff)); <--- this is a bad approach. Better to use select/epoll
        int num_events = epoll_wait(epoll_fd, events, MAX, -1);
        if (num_events == -1) {
            continue;
        }
        for (int i = 0; i < num_events; i++) {
            if (events[i].data.fd == fd[0]) {
                read(fd[0], buff, MAX_BUFFER_SIZE);
                // sending other messages to client
                write(connfd, buff, MAX_BUFFER_SIZE);

            } else if (events[i].data.fd == connfd) {
                read(connfd, buff, sizeof(buff));
                // send message to all other pipes
                for (int i = 0; i < num_clients; i++) {
                    if (i != client_i) {
                        write(pipes[i][1], buff, MAX_BUFFER_SIZE);
                    }
                }
            }


        }

        memset(&events, 0, sizeof(events));
    }
    // PREVIOUS CODE:
        // for (int i = 0; i < num_clients; i++) {
        //     if (clients[i] == connfd) {
        //         while (i < num_clients - 1) {
        //             clients[i] = clients[i + 1];
        //             i++;
        //         }
        //         break;
        //     }
        // }
    // NOW:
    for (int i = 0; i < num_clients; i++) {
        if (i == client_i) {
            while (i < num_clients - 1) {
                clients[i] = clients[i + 1];
                pipes[i][0] = pipes[i + 1][0];
                pipes[i][1] = pipes[i + 1][1];
                i++;
            }
            break;
        }
    }

    num_clients--;
    close(connfd);
    pthread_exit(NULL);
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    pthread_t tid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    } else
        printf("Socket successfully created..\n");

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    } else
        printf("Socket successfully binded..\n");

    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    } else
        printf("Server listening..\n");

    while (1) {
        socklen_t len = sizeof(cli);
        connfd = accept(sockfd, (SA*)&cli, &len);
        if (connfd < 0) {
            printf("server accept failed...\n");
            exit(0);
        }

        printf("server accept the client...\n");

        pthread_create(&tid, NULL, func, &connfd);
    }

    close(sockfd);
    return 0;
}

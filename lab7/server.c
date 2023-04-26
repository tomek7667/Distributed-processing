#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int clients[5];
int num_clients = 0;

void* func(void* arg)
{
    int connfd = *(int*)arg;
    clients[num_clients++] = connfd; // adding client to list
    char buff[MAX];
    int n;

    for (;;) {
        bzero(buff, MAX);
        read(connfd, buff, sizeof(buff));
        // if buff is CTRL+C or CTRL+Z or string 'exit', then we quit
        if (buff[0] == 0 || buff[0] == 3 || buff[0] == 26 || strncmp("exit", buff, 4) == 0) {
            printf("Client Exit...\n");
            break;
        }

        printf("From client: %s\n", buff);

        // sending messages
        for (int i = 0; i < num_clients; i++) {
            if (clients[i] != connfd) { // don't send to client who sent the message
                write(clients[i], buff, sizeof(buff));
            }
        }

        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }

    for (int i = 0; i < num_clients; i++) {
        if (clients[i] == connfd) {
            while (i < num_clients - 1) {
                clients[i] = clients[i + 1];
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

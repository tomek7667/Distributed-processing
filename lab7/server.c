#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#include <pthread.h> // threads
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int clients[5]; // maksymalnie 5 klientów
int num_clients = 0;

void *func(void *arg) // funkcja dla każdego wątku
{
    int connfd = *(int*)arg;
    clients[num_clients++] = connfd; // dodajemy klienta do listy
    char buff[MAX];
    int n;

    for (;;) {
        bzero(buff, MAX);
        read(connfd, buff, sizeof(buff));

        printf("From client: %s\n", buff);

        // przesyłamy wiadomość do pozostałych klientów
        for (int i = 0; i < num_clients; i++) {
            if (clients[i] != connfd) { // pominę klienta, który wysłał wiadomość
                write(clients[i], buff, sizeof(buff));
            }
        }

        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }

    // usuwamy klienta z listy
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
    }
    else
        printf("Socket successfully created..\n");

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

    while (1) {
        socklen_t len = sizeof(cli);
        connfd = accept(sockfd, (SA*)&cli, &len);
        if (connfd < 0) {
            printf("server accept failed...\n");
            exit(0);
        }

        printf("server accept the client...\n");

        pthread_create(&tid, NULL, func, &connfd); // tworzymy wątek dla każdego nowego klienta
    }

    close(sockfd);
    return 0;
}

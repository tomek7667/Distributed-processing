#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX 80
#define SA struct sockaddr
char temp_message_buffer[MAX];

int clients[5];
int num_clients = 0;

sem_t message_sem;

void clear_temp_buffer() {
    for (int i = 0; i < MAX; i++) {
        temp_message_buffer[i] = 0;
    }
}


void* receive(void* arg)
{
    int connfd = *(int*)arg;
    clients[num_clients++] = connfd; // adding client to list
    char buff[MAX];
    int n;

    while (1) {
        bzero(buff, MAX);
        read(connfd, buff, sizeof(buff));
        // if buff is CTRL+C or CTRL+Z or string 'exit', then we quit
        if (buff[0] == 0 || buff[0] == 3 || buff[0] == 26 || strncmp("exit", buff, 4) == 0) {
            printf("Client Exit...\n");
            break;
        }

        printf("From client: %s;", buff);

        // TU KLIENT DOSTAJE COS, WIEC TEZ MA WYSLAC INFO DO INNYCH WATKOW ZEBY WYSLALI `buff`
        // temp_message_buffer = buff
        
        clear_temp_buffer();
        strcpy(temp_message_buffer, buff);
        sem_post(&message_sem);
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

void* send_message(void* arg)
{
    int connfd = *(int*)arg;
    while (1) {
        sem_wait(&message_sem);
        write(connfd, temp_message_buffer, sizeof(temp_message_buffer));
    }
}

int main(int argc, char* argv[]) {
    sem_init(&message_sem, 0, 0);
    int port = atoi(argv[1]);
    printf("listening on %i\n", port);
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
    servaddr.sin_port = htons(port);

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

        pthread_create(&tid, NULL, receive, &connfd);
        pthread_create(&tid, NULL, send_message, &connfd);
    }

    close(sockfd);
    return 0;
}

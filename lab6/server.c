#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <pthread.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 100
#define MAX_USERNAME_LEN 20
#define MAX_MESSAGE_LEN 1000

const char* PREFIX = "SERVER:";
const char* WELCOME_MESSAGE = "SERVER:Welcome to the chat!\nIn order to disconnect type 'q'\nif you want to send a private message, type '/m <username> <message>'\nTo list current users type '/l'\n";
const char* USERNAME_REGEX = "^[a-z]+$";
const char* WRONG_USERNAME = "SERVER:Please choose another name. (must match regex: " USERNAME_REGEX " and not already exist)";

struct client_info {
    int sockfd;
    char username[MAX_USERNAME_LEN + 1];
    struct sockaddr_in addr;
};

struct client_info clients[MAX_CLIENTS];

void send_to_all(char* msg, int sender_sockfd) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].sockfd > 0 && clients[i].sockfd != sender_sockfd) {
            send(clients[i].sockfd, msg, strlen(msg), 0);
        }
    }
}

void send_to_all_except(char* msg, int sender_sockfd, char* username) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].sockfd > 0 && clients[i].sockfd != sender_sockfd && strcmp(clients[i].username, username) != 0) {
            send(clients[i].sockfd, msg, strlen(msg), 0);
        }
    }
}

void log_msg(char* msg) {
    printf("LOG: %s\n", msg);
}

void* handle_client(void* arg) {
    struct client_info client = *((struct client_info*)arg);
    int sockfd = client.sockfd;
    char* username = client.username;
    struct sockaddr_in addr = client.addr;

    char buf[MAX_MESSAGE_LEN + 1];
    int n;

    // Receive username from the client
    n = recv(sockfd, username, MAX_USERNAME_LEN, 0);
    if (n < 0) {
        perror("recv");
        close(sockfd);
        return NULL;
    }
    username[n] = '\0';

    // Validate username
    if (strlen(username) == 0 || !islower(username[0]) || !isalpha(username[0]) || strlen(username) > MAX_USERNAME_LEN) {
        send(sockfd, WRONG_USERNAME, strlen(WRONG_USERNAME), 0);
        close(sockfd);
        return NULL;
    }

    // Check if the username already exists
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].sockfd > 0 && strcmp(clients[i].username, username) == 0) {
            send(sockfd, WRONG_USERNAME, strlen(WRONG_USERNAME), 0);
            close(sockfd);
            return NULL;
        }
    }

    // Add client to the clients array
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].sockfd == 0) {
            clients[i] = client;
            break;
        }
    }

    // Send welcome message to the client
    send(sockfd, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE), 0);
    printf("%s has joined the chat.\n", username);

    while (1) {
        char buffer[1024];
        int received = recv(conn, buffer, 1024, 0);
        if (received <= 0) {
            remove_user(username);
            printf("%s has left the chat.\n", username);
            send_to_all(user_leave_message(username));
            close(conn);
            break;
        }
        buffer[received] = '\0';

        if (strncmp(buffer, "/m", 2) == 0) {
            handle_private_message(buffer, username);
        } else if (strncmp(buffer, "/l", 2) == 0) {
            handle_list_users(conn);
        } else {
            send_to_all_except(format_message(buffer, username), username);
        }
    }

    return NULL;
}

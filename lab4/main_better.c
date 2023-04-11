#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define NUM_THREADS 1
#define NUM_ELEMENTS 10000
#define INITIAL_NUM_START -600
#define INITIAL_NUM_END 600

typedef struct node {
    int data;
    struct node* next;
} node;

node* head = NULL;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* addToList(void* arg) {
    int id = *(int*)arg;
    int start = id * NUM_ELEMENTS;
    int end = start + NUM_ELEMENTS;

    for (int i = start; i < end; i++) {
        node* newNode = (node*)malloc(sizeof(node));
        newNode->data = i;
        newNode->next = NULL;

        pthread_mutex_lock(&mutex);
        if (head == NULL) {
            head = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void* removeFromList(void* arg) {
    int id = *(int*)arg;
    int start = id * NUM_ELEMENTS;
    int end = start + NUM_ELEMENTS;

    for (int i = start; i < end; i++) {
        pthread_mutex_lock(&mutex);
        node* prev = NULL;
        node* curr = head;
        while (curr != NULL && curr->data != i) {
            prev = curr;
            curr = curr->next;
        }
        if (curr != NULL) {
            if (prev == NULL) {
                head = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    for (int i = INITIAL_NUM_START; i < INITIAL_NUM_END; i++) {
        node* newNode = (node*)malloc(sizeof(node));
        newNode->data = i;
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, addToList, &thread_ids[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    int count = 0;
    node* curr = head;
    while (curr != NULL) {
        // printf("%d\n", curr->data);
        curr = curr->next;
        count++;
    }
    printf("Number of elements in the list:\n%d\n", count);
    printf("Removing elements from the list...\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, removeFromList, &thread_ids[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    if (head == NULL) {
        printf("List is empty\n");
    } else {
        count = 0;
        curr = head;
        while (curr != NULL) {
            // printf("%d\n", curr->data);
            curr = curr->next;
            count++;
        }
        printf("Number of elements in the list (after deleting):\n%d\n", count);
    }
    return 0;
}

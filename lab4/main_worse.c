#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

struct node {
    struct node* previous;
    struct node* next;
    void* value;
} *begin;

struct listNode {
    struct node* current;
    bool (*comparator)(void*, void*);
    void (*printFunction)(void*);
    struct node* (*allocateFunction)(void*);
    void (*deallocateFunction)(struct node*);
    pthread_mutex_t mutex;
};

typedef struct argsStruct {
    struct listNode* list;
    void* value;
};

struct listNode* createList(void* value, bool (*comparator)(void*, void*), void (*printFunction)(void*), struct node* (*allocateFunction)(void*), void (*deallocateFunction)(struct node*)) {
    struct listNode* list = (struct listNode*)malloc(sizeof(struct listNode));
    list->current = allocateFunction(value);
    list->current->previous = NULL;
    list->current->next = NULL;
    list->current->value = value;
    list->comparator = comparator;
    list->printFunction = printFunction;
    list->allocateFunction = allocateFunction;
    list->deallocateFunction = deallocateFunction;
    pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;
    list->mutex = _mutex;
    return list;
}

void* insert(struct argsStruct args) {
    void* newValue = args.value;
    struct listNode* list = args.list;
    pthread_mutex_lock(&(list->mutex));
    struct node* pointer = list->current;

    if (list->comparator(newValue, pointer->value)) {
        // if new is larger
        if (pointer->next == NULL) {
            pointer->next = list->allocateFunction(newValue);
            pointer->next->previous = pointer;
            pointer->next->value = newValue;
            pthread_mutex_unlock(&(list->mutex));
            pthread_exit(NULL);
            return pointer->next;
        } else if (list->comparator(pointer->next->value, newValue) || newValue == pointer->value) {
            struct node* newNode = list->allocateFunction(newValue);
            newNode->previous = pointer;
            newNode->next = pointer->next;
            newNode->value = newValue;
            pointer->next->previous = newNode;
            pointer->next = newNode;
            pthread_mutex_unlock(&(list->mutex));
            pthread_exit(NULL);
            return newNode;
        } else {
            list->current = list->current->next;
            struct argsStruct newArgs = {list, newValue};
            return insert(newArgs);
        }
    } else {
        // if old is larger
        if (pointer->previous == NULL) {
            pointer->previous = list->allocateFunction(newValue);
            pointer->previous->next = pointer;
            pointer->previous->value = newValue;
            pthread_mutex_unlock(&(list->mutex));
            pthread_exit(NULL);
            return pointer->previous;
        } else if (list->comparator(newValue, pointer->previous->value) || newValue == pointer->value) {
            struct node* newNode = list->allocateFunction(newValue);
            newNode->next = pointer;
            newNode->previous = pointer->previous;
            newNode->value = newValue;
            pointer->previous->next = newNode;
            pointer->previous = newNode;
            pthread_mutex_unlock(&(list->mutex));
            pthread_exit(NULL);
            return newNode;
        } else {
            list->current = list->current->previous;
            struct argsStruct newArgs = {list, newValue};
            return insert(newArgs);
        }
    }
}

struct node* removeNode(struct node* node, struct listNode* list) {
    pthread_mutex_lock(&(list->mutex));
    if (node->previous == NULL) {
        node->next->previous = NULL;
        pthread_mutex_unlock(&(list->mutex));
        pthread_exit(NULL);
        return node->next;
    } else if (node->next == NULL) {
        node->previous->next = NULL;
        pthread_mutex_unlock(&(list->mutex));
        pthread_exit(NULL);
        return node->previous;
    } else {
        node->previous->next = node->next;
        node->next->previous = node->previous;
        pthread_mutex_unlock(&(list->mutex));
        pthread_exit(NULL);
        return node->next;
    }
}

void traverse(struct listNode* listNode) {
    printf("Traversing:\n");
    // firstly get the begin
    struct node** begin = &(listNode->current);
    while ((*begin)->previous != NULL) {
        begin = &((*begin)->previous);
    }
    if ((*begin)->previous != NULL) {
        printf("error\n");
        return;
    }
    int i = 1;
    while ((*begin)->next) {
        printf("%i. data: ", i);
        listNode->printFunction((*begin)->value);
        begin = &((*begin)->next);
        i++;
    }
    printf("%i. data: ", i);
    listNode->printFunction((*begin)->value);
    return;
}

void traverseDealloc(struct listNode** list) {
    // printf("Freeing program memory:\n");
    struct node** begin = &((*list)->current);
    while ((*begin)->previous != NULL) {
        begin = &((*begin)->previous);
    }
    struct node* current = *begin;
    struct node* next = current->next;
    while (next) {
        // Uncomment following to see the debug output
        // printf("freeing: ");
        // (*list)->printFunction(current->value);
        (*list)->deallocateFunction(current);
        current = next;
        next = current->next;
    }
    // Uncomment following to see the debug output
    // printf("freeing: ");
    // (*list)->printFunction(current->value);
    (*list)->deallocateFunction(current);
    return;
}

void printInts(void* a) {
    int b = *(int*)a;
    printf("int: %i\n", b);
}

bool compareInts(void* isThisBigger, void* thanThis) {
    return *(int*)isThisBigger > *(int*)thanThis;
}

void deallocateIntNode(struct node* node) {
    free(node);
}

struct node* allocateIntNode(void* value) {
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->previous = NULL;
    newNode->next = NULL;
    newNode->value = (int*)malloc(sizeof(int));
    *(int*)newNode->value = *(int*)value;
    return newNode;
}

void printSpacer() {
    printf("+=+=+=+=+=+=+=+=+=+\n");
}

#define NUM_OF_THREADS 10
#define NUM_OF_ELEMENTS 10000

int main() {
    int* currentInt = (int*)malloc(sizeof(int));
    *currentInt = 3;
    // Initializing the list with specifying `int` as the type
    // with required functions for the list to work
    struct listNode* list = createList(currentInt, compareInts, printInts, allocateIntNode, deallocateIntNode);
    
    // THREADING
    pthread_t threads[NUM_OF_THREADS];
    int threadIds[NUM_OF_THREADS];

    for (int i = 0; i < NUM_OF_THREADS; i++) {
        threadIds[i] = i;
        for (int val = 0; val < NUM_OF_ELEMENTS; val++) {
            struct argsStruct a = {list, val};
            pthread_create(&threads[i], NULL, insert, &a);
        }
    }
    for (int i = 0; i < NUM_OF_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    traverse(list);
    


    // printSpacer();
    traverseDealloc(&list);
    return 0;
}
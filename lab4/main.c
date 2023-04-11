#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define NUM_OF_THREADS 5
#define NUM_OF_ELEMENTS 3

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
    pthread_mutex_t* mutex;
};

struct argsStruct {
    struct listNode* list;
    int threadNumber;
};

void printNeighborsOf(struct node* current, void printFunction(void*)) {
    if (current->previous == NULL) {
        printf("previous=NULL ");
    } else {
        printf("previous=");
        printFunction(current->previous->value);
    }

    printf("current=");
    printFunction(current->value);

    if (current->next == NULL) {
        printf(" next=NULL\n");
    } else {
        printf(" next=");
        printFunction(current->next->value);
        printf("\n");
    }
}


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
    list->mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(list->mutex, NULL);
    return list;
}

void* insert(void* newValue, struct listNode* list, int threadNumber) {
    struct node* pointer = list->current;

    // pthread_mutex_lock(list->mutex);
    if (list->comparator(newValue, pointer->value)) {
        // if new is larger
        if (pointer->next == NULL) {
            pointer->next = list->allocateFunction(newValue);
            pointer->next->previous = pointer;
            pointer->next->value = newValue;
            // pthread_mutex_unlock((list->mutex));
            // pthread_exit(NULL);
            // return pointer->next;
        } else if (list->comparator(pointer->next->value, newValue) || newValue == pointer->value) {
            struct node* newNode = list->allocateFunction(newValue);
            newNode->previous = pointer;
            newNode->next = pointer->next;
            newNode->value = newValue;
            pointer->next->previous = newNode;
            pointer->next = newNode;
            // pthread_mutex_unlock((list->mutex));
            // pthread_exit(NULL);
            // return newNode;
        } else {
            list->current = list->current->next;
            // return insert(newArgs);
            // pthread_mutex_unlock((list->mutex));
            // pthread_exit(NULL);
            insert(newValue, list, threadNumber);
        }
    } else {
        // if old is larger
        if (pointer->previous == NULL) {
            pointer->previous = list->allocateFunction(newValue);
            pointer->previous->next = pointer;
            pointer->previous->value = newValue;
            // pthread_mutex_unlock((list->mutex));
            // return pointer->previous;
        } else if (list->comparator(newValue, pointer->previous->value) || newValue == pointer->value) {
            struct node* newNode = list->allocateFunction(newValue);
            newNode->next = pointer;
            newNode->previous = pointer->previous;
            newNode->value = newValue;
            pointer->previous->next = newNode;
            pointer->previous = newNode;
            // pthread_mutex_unlock((list->mutex));
            // pthread_exit(NULL);
            // return newNode;
        } else {
            list->current = list->current->previous;
            // pthread_exit(NULL);
            // pthread_mutex_unlock((list->mutex));
            insert(newValue, list, threadNumber);
            // return insert(newArgs);
        }
    }
}

void* removeNode(struct node* node, struct listNode* list) {
    pthread_mutex_lock((list->mutex));
    if (node->previous == NULL) {
        node->next->previous = NULL;
        pthread_mutex_unlock((list->mutex));
        pthread_exit(NULL);
        // return node->next;
    } else if (node->next == NULL) {
        node->previous->next = NULL;
        pthread_mutex_unlock((list->mutex));
        pthread_exit(NULL);
        // return node->previous;
    } else {
        node->previous->next = node->next;
        node->next->previous = node->previous;
        pthread_mutex_unlock((list->mutex));
        pthread_exit(NULL);
        // return node->next;
    }
}

void traverse(struct listNode* list) {
    printf("Traversing:\n");
    // firstly get the begin
    struct node** begin = &((list)->current);
    
    list->printFunction((*begin)->value);
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
        (list)->printFunction((*begin)->value);
        begin = &((*begin)->next);
        i++;
    }
    printf("%i. data: ", i);
    (list)->printFunction((*begin)->value);
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

void *insertAsThread(void* args) {
    struct listNode* list = ((struct argsStruct*)args)->list;
    int threadId = ((struct argsStruct*)args)->threadNumber;
    int _array[NUM_OF_ELEMENTS];
    for (int i = 1; i < NUM_OF_ELEMENTS + 1; i++) {
        _array[i-1] = i;
    }


    for (int i = 0; i < NUM_OF_ELEMENTS; i++) {
        // printf("Thread %i inserting %i\n", threadId, _array[i]);
        pthread_mutex_lock(list->mutex);
        insert(&_array[i], list, threadId);
        pthread_mutex_unlock(list->mutex);
    }
    pthread_exit(NULL);
}

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
        printf("Creating thread %i\n", i);
        struct argsStruct* newArgs = (struct argsStruct*)malloc(sizeof(struct argsStruct));
        newArgs->list = list;
        newArgs->threadNumber = i;
        pthread_create(&threads[i], NULL, insertAsThread, (void*)newArgs);
    }
    for (int i = 0; i < NUM_OF_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    traverse(list);
    traverseDealloc(&list);
    return 0;
}
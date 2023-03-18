/* Distributed Programming
   Lab2 - libraries - example

   (C) by Mariusz Matuszek
   Gdańsk University of Technology
*/


// function definition(s)

#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int add(data_t x, data_t y) {
  return(x.a + x.b + y.a + y.b);
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
    return list;
}

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

struct node* allocateIntNode(void* value) {
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->previous = NULL;
    newNode->next = NULL;
    newNode->value = (int*)malloc(sizeof(int));
    *(int*)newNode->value = *(int*)value;
    return newNode;
}

struct node* allocateFloatNode(void* value) {
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->previous = NULL;
    newNode->next = NULL;
    newNode->value = (float*)malloc(sizeof(float));
    *(float*)newNode->value = *(float*)value;
    return newNode;
}

struct node* allocateDoubleNode(void* value) {
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->previous = NULL;
    newNode->next = NULL;
    newNode->value = (double*)malloc(sizeof(double));
    *(double*)newNode->value = *(double*)value;
    return newNode;
}

struct node* allocateStringNode(void* value) {
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->previous = NULL;
    newNode->next = NULL;
    newNode->value = (char*)malloc(sizeof(char) * MAX_STRING_LENGTH);
    strcpy((char*)newNode->value, (char*)value);
    return newNode;
}

struct node* allocatePersonNode(void* value) {
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->previous = NULL;
    newNode->next = NULL;
    newNode->value = value;
    return newNode;
}

void deallocateIntNode(struct node* node) {
    free(node);
}

void deallocateFloatNode(struct node* node) {
    free(node);
}

void deallocateDoubleNode(struct node* node) {
    free(node);
}

void deallocateStringNode(struct node* node) {
    char *s = (char*)node->value;
    free(s);
    free(node);
}

void deallocatePersonNode(struct node* node) {
    struct person *p = (struct person*)node->value;
    char *s = (char*)p->name;
    free(s);
    free(node);
}

struct node* insert(void* newValue, struct listNode* list) {
    struct node* pointer = list->current;

    if (list->comparator(newValue, pointer->value)) {
        // if new is larger
        if (pointer->next == NULL) {
            pointer->next = list->allocateFunction(newValue);
            pointer->next->previous = pointer;
            pointer->next->value = newValue;
            return pointer->next;
        } else if (list->comparator(pointer->next->value, newValue) || newValue == pointer->value) {
            struct node* newNode = list->allocateFunction(newValue);
            newNode->previous = pointer;
            newNode->next = pointer->next;
            newNode->value = newValue;
            pointer->next->previous = newNode;
            pointer->next = newNode;
            return newNode;
        } else {
            list->current = list->current->next;
            return insert(newValue, list);
        }
    } else {
        // if old is larger
        if (pointer->previous == NULL) {
            pointer->previous = list->allocateFunction(newValue);
            pointer->previous->next = pointer;
            pointer->previous->value = newValue;
            return pointer->previous;
        } else if (list->comparator(newValue, pointer->previous->value) || newValue == pointer->value) {
            struct node* newNode = list->allocateFunction(newValue);
            newNode->next = pointer;
            newNode->previous = pointer->previous;
            newNode->value = newValue;
            pointer->previous->next = newNode;
            pointer->previous = newNode;
            return newNode;
        } else {
            list->current = list->current->previous;    
            return insert(newValue, list);
        }
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
    printf("Freeing program memory:\n");
    struct node** begin = &((*list)->current);
    while ((*begin)->previous != NULL) {
        begin = &((*begin)->previous);
    }
    struct node* current = *begin;
    struct node* next = current->next;
    while (next) {
        printf("freeing: ");
        (*list)->printFunction(current->value);
        // free(current);
        (*list)->deallocateFunction(current);
        current = next;
        next = current->next;
    }
    printf("freeing: ");
    (*list)->printFunction(current->value);
    // free(current);
    (*list)->deallocateFunction(current);
    return;
}

bool compareInts(void* isThisBigger, void* thanThis) {
    return *(int*)isThisBigger > *(int*)thanThis;
}

bool compareFloats(void* isThisBigger, void* thanThis) {
    return *(float*)isThisBigger > *(float*)thanThis;
}

bool compareDoubles(void* isThisBigger, void* thanThis) {
    return *(double*)isThisBigger > *(double*)thanThis;
}

bool compareStrings(void* isThisBigger, void* thanThis) {
    return strlen((char*)isThisBigger) > strlen((char*)thanThis);
}

bool comparePersons(void* isThisBigger, void* thanThis) {
    // If is woman, then it have a priority
    struct person* p1 = (struct person*)isThisBigger;
    struct person* p2 = (struct person*)thanThis;
    if (p1->isMale != p2->isMale) {
        return p1->isMale;
    }
    return p1->age > p2->age;
}

void printSpacer() {
    printf("+=+=+=+=+=+=+=+=+=+\n");
}

void printInts(void* a) {
    int b = *(int*)a;
    printf("int: %i\n", b);
}

void printFloats(void* a) {
    float b = *(float*)a;
    printf("float: %f\n", b);
}

void printDoubles(void* a) {
    double b = *(double*)a;
    printf("double: %f\n", b);
}

void printStrings(void* a) {
    char* b = (char*)a;
    printf("string: %s\n", b);
}

void printPersons(void* a) {
    struct person* b = (struct person*)a;
    if (b->isMale) {
        printf("Man ");
    } else {
        printf("Woman ");
    }
    printf("named %s, aged %i\n", b->name, b->age);
}

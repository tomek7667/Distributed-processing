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
    newNode->value = (char*)malloc(sizeof(char) * 100);
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

struct node* insert(void* newValue, struct node* pointer, bool (*comparator)(), void printFunction(void*), struct node* (allocateFunction(void*))) {
    if (comparator(newValue, pointer->value)) {
        // printf("A");
        // if new is larger
        if (pointer->next == NULL) {
            // printf("A\n");
            pointer->next = allocateFunction(newValue);
            pointer->next->previous = pointer;
            pointer->next->value = newValue;
            return pointer->next;
        } else if (comparator(pointer->next->value, newValue) || newValue == pointer->value) {
            // printf("B\n");
            struct node* newNode = allocateFunction(newValue);
            newNode->previous = pointer;
            newNode->next = pointer->next;
            newNode->value = newValue;
            pointer->next->previous = newNode;
            pointer->next = newNode;
            return newNode;
        } else {
            // printf("C\n");
            return insert(newValue, pointer->next, comparator, printFunction, allocateFunction);
        }
    } else {
        // printf("B");
        // if old is larger
        if (pointer->previous == NULL) {
            // printf("A\n");
            pointer->previous = allocateFunction(newValue);
            pointer->previous->next = pointer;
            pointer->previous->value = newValue;
            return pointer->previous;
        } else if (comparator(newValue, pointer->previous->value) || newValue == pointer->value) {
            // printf("B\n");
            struct node* newNode = allocateFunction(newValue);
            newNode->next = pointer;
            newNode->previous = pointer->previous;
            newNode->value = newValue;
            pointer->previous->next = newNode;
            pointer->previous = newNode;
            return newNode;
        } else {
            // printf("C\n");
            return insert(newValue, pointer->previous, comparator, printFunction, allocateFunction);
        }
    }
}

void traverse(struct node* pointer, void printFunction(void*)) {
    printf("Traversing:\n");
    // firstly get the begin
    struct node** begin = &pointer;
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
        printFunction((*begin)->value);
        begin = &((*begin)->next);
        i++;
    }
    printf("%i. data: ", i);
    printFunction((*begin)->value);
    return;
}

void traverseDealloc(struct node** pointer, void printFunction(void*), void deallocateFunction(struct node*)) {
    printf("Freeing program memory:\n");
    struct node** begin = pointer;
    while ((*begin)->previous != NULL) {
        begin = &((*begin)->previous);
    }
    struct node* current = *begin;
    struct node* next = current->next;
    while (next) {
        printf("freeing: ");
        printFunction(current->value);
        // free(current);
        deallocateFunction(current);
        current = next;
        next = current->next;
    }
    printf("freeing: ");
    printFunction(current->value);
    // free(current);
    deallocateFunction(current);
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

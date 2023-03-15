#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_STRING_LENGTH 100

struct person {
    char name[MAX_NAME_LENGTH];
    int age;
    bool isMale;
};

struct node {
    struct node* previous;
    struct node* next;
    void* value;
} *begin;

void printNeighborsOf(struct node* current, void printFunction(void*)) {
    if (current->previous == NULL) {
        printf("previous=NULL ");
    } else {
        // printf("previous=%i ", current->previous->value);
        printf("previous=");
        printFunction(current->previous->value);
    }

    // printf("current=%i ", current->value);
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

void runInts() {
    // Ints
    // initialize current node
    int* c = (int*)malloc(sizeof(int));
    *c = 3;
    struct node* current = allocateIntNode(c);
    // insert values
    int ints[9] = { 2, 1, 6, 12, 5, 4, 12, 4, 12 };
    for (int i = 0; i < 9; i++) {
        current = insert(&ints[i], current, compareInts, printInts, allocateIntNode);
    }
    traverse(current, printInts);
    printSpacer();
    traverseDealloc(&current, printInts, deallocateIntNode);
}

void runFloats() {
    // Floats
    // initialize current node
    float* c = (float*)malloc(sizeof(float));
    *c = 3.3;
    struct node* current = allocateFloatNode(c);
    float floats[9] = { 2.2, 1.1, 6.6, 12.12, 5.5, 4.4, 12.12, 4.4, 12.12 };
    for (int i = 0; i < 9; i++) {
        current = insert(&floats[i], current, compareFloats, printFloats, allocateFloatNode);
    }
    traverse(current, printFloats);
    printSpacer();
    traverseDealloc(&current, printFloats, deallocateFloatNode);
    // insert values
}

void runDoubles() {
    // Doubles
    // initialize current node
    double* c = (double*)malloc(sizeof(double));
    *c = 3.3;
    struct node* current = allocateDoubleNode(c);
    // insert values
    double doubles[9] = { 2.2, 1.1, 6.6, 12.12, 5.5, 4.4, 12.12, 4.4, 12.12 };
    for (int i = 0; i < 9; i++) {
        current = insert(&doubles[i], current, compareDoubles, printDoubles, allocateDoubleNode);
    }
    traverse(current, printDoubles);
    printSpacer();
    traverseDealloc(&current, printDoubles, deallocateDoubleNode);
}

void runStrings() {
    // Strings
    // initialize current node
    char* c = (char*)malloc(sizeof(char) * MAX_STRING_LENGTH);
    strcpy(c, "test");
    struct node* current = allocateStringNode(c);
    // insert values
    char* strings[MAX_STRING_LENGTH] = {
        "shrt",
        "loooong",
        "modrt",
        "this is a sentence",
        "how about such long sequence?! :O",
        "s",
        "this is moderate",
        "pancakes",
        "55",
    };

    for (int i = 0; i < 9; i++) {
        char* str = (char*)malloc(sizeof(char) * MAX_STRING_LENGTH);
        strcpy(str, strings[i]);
        current = insert(str, current, compareStrings, printStrings, allocateStringNode);
    }
    traverse(current, printStrings);
    printSpacer();
    traverseDealloc(&current, printStrings, deallocateStringNode);
}

void runPersons() {
    // Persons
    // initialize current node
    struct person* c = (struct person*)malloc(sizeof(struct person));
    c->age = 3;
    strcpy(c->name, "Noe");
    c->isMale = true;
    struct node* current = allocatePersonNode(c);
    // insert values
    struct person persons[9] = {
        { "Marcus", 21, true },
        { "Marcin", 33, true },
        { "Jerzy", 40, true },
        { "Wacław", 19, true },
        { "Maria", 25, false },
        { "Janina", 21, false },
        { "Jurek", 30, true },
        { "Róża", 72, false },
        { "Lucjan", 13, true },
    };

    for (int i = 0; i < 9; i++) {
        struct person* p = (struct person*)malloc(sizeof(struct person));
        p->age = persons[i].age;
        strcpy(p->name, persons[i].name);
        p->isMale = persons[i].isMale;
        current = insert(p, current, comparePersons, printPersons, allocatePersonNode);
    }
    traverse(current, printPersons);
    printSpacer();
    traverseDealloc(&current, printPersons, deallocatePersonNode);
}

int main() {
    printSpacer();
    runInts();
    printSpacer();
    runFloats();
    printSpacer();
    runDoubles();
    printSpacer();
    runStrings();
    printSpacer();
    runPersons();
    printSpacer();
    return 0;
}
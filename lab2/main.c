/* Distributed Programming
   Lab2 - libraries - example

   (C) by Mariusz Matuszek
   Gdańsk University of Technology
*/


// main app

#include "types.h"
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void runInts() {
    // Ints
    // initialize current node
    int* currentInt = (int*)malloc(sizeof(int));
    *currentInt = 3;
    struct listNode* list = createList(currentInt, compareInts, printInts, allocateIntNode, deallocateIntNode);
    // insert values
    int ints[10] = { 2, 1, 6, 12, 5, 4, 12, 4, 12, 7 };
    for (int i = 0; i < 10; i++) {
        list->current = insert(&ints[i], list);
    }
    traverse(list);
    printSpacer();
    traverseDealloc(&list);
}

void runFloats() {
    // Floats
    // initialize current node
    float* currentFloat = (float*)malloc(sizeof(float));
    *currentFloat = 3.5;
    struct listNode* list = createList(currentFloat, compareFloats, printFloats, allocateFloatNode, deallocateFloatNode);
    // insert values
    float floats[10] = { 2.5, 1.5, 6.5, 12.5, 5.5, 4.5, 12.5, 4.5, 12.5, 7.5 };
    for (int i = 0; i < 10; i++) {
        list->current = insert(&floats[i], list);
    }
    traverse(list);
    printSpacer();
    traverseDealloc(&list);
}

void runDoubles() {
    // Doubles
    // initialize current node
    double* currentDouble = (double*)malloc(sizeof(double));
    *currentDouble = 3.5;
    struct listNode* list = createList(currentDouble, compareDoubles, printDoubles, allocateDoubleNode, deallocateDoubleNode);
    // insert values
    double doubles[10] = { 2.5, 1.5, 6.5, 12.5, 5.5, 4.5, 12.5, 4.5, 12.5, 7.5 };
    for (int i = 0; i < 10; i++) {
        list->current = insert(&doubles[i], list);
    }
    traverse(list);
    printSpacer();
    traverseDealloc(&list);
}

void runPersons() {
    // Persons
    // initialize current node
    struct person* c = (struct person*)malloc(sizeof(struct person));
    c->age = 3;
    strcpy(c->name, "Noe");
    c->isMale = true;
    struct listNode* list = createList(c, comparePersons, printPersons, allocatePersonNode, deallocatePersonNode);
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
        list->current = insert(p, list);
    }
    traverse(list);
    printSpacer();
    traverseDealloc(&list);
}

int main(int argc, char* argv[]) {

    data_t x, y;

    x.a = x.b = y.a = y.b = 1;

    printf("wynik dodawania: %d\n", add(x, y));


    printSpacer();
    runInts();
    printSpacer();
    runFloats();
    printSpacer();
    runDoubles();
    printSpacer();
    runPersons();
    printSpacer();
    printf("Program finished successfully!\n");
    return 0;
}

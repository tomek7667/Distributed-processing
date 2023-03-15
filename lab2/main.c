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
    char* c = (char*)malloc(sizeof(char) * 100);
    strcpy(c, "test");
    struct node* current = allocateStringNode(c);
    // insert values
    char* strings[100] = {
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
        char* str = (char*)malloc(sizeof(char) * 100);
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
  runStrings();
  printSpacer();
  runPersons();
  printSpacer();
  return 0;
}

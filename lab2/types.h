/* Distributed Programming
   Lab2 - libraries - example

   (C) by Mariusz Matuszek
   Gdańsk University of Technology
*/


// protect against 'multiple definition' error when including more than once
#ifndef _TYPES_H_
#define _TYPES_H_
#include <stdbool.h>
#define MAX_NAME_LENGTH 50
#define MAX_STRING_LENGTH 100

typedef struct data {
  int a;
  int b;
} data_t;

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

struct listNode {
    struct node* current;
    bool (*comparator)(void*, void*);
    void (*printFunction)(void*);
    struct node* (*allocateFunction)(void*);
    void (*deallocateFunction)(struct node*);
};

#endif

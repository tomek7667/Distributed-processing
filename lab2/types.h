/* Distributed Programming
   Lab2 - libraries - example

   (C) by Mariusz Matuszek
   Gdańsk University of Technology
*/


// protect against 'multiple definition' error when including more than once
#ifndef _TYPES_H_
#define _TYPES_H_
#include <stdbool.h>

typedef struct data {
  int a;
  int b;
} data_t;

typedef struct person {
    char name[100];
    int age;
    bool isMale;
};

typedef struct node {
    struct node* previous;
    struct node* next;
    void* value;
} *begin;

#endif

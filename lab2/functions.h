/* Distributed Programming
   Lab2 - libraries - example

   (C) by Mariusz Matuszek
   Gdańsk University of Technology
*/


// protect against 'multiple definition' error when including more than once
#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H

#include "types.h"

int add(data_t x, data_t y);

// Universal functions
struct node* insert(void* newValue, struct node* pointer, bool (*comparator)(), void printFunction(void*), struct node* (allocateFunction(void*)));
void traverse(struct node* pointer, void printFunction(void*));
void traverseDealloc(struct node** pointer, void printFunction(void*), void deallocateFunction(struct node*));
void printNeighborsOf(struct node* current, void printFunction(void*));

/// Customised functions

// Custom comparators
bool compareInts(void* isThisBigger, void* thanThis);
bool compareFloats(void* isThisBigger, void* thanThis);
bool compareDoubles(void* isThisBigger, void* thanThis);
bool compareStrings(void* isThisBigger, void* thanThis);
bool comparePersons(void* isThisBigger, void* thanThis);

// Custom allocation functions
struct node* allocateIntNode(void* value);
struct node* allocateFloatNode(void* value);
struct node* allocateDoubleNode(void* value);
struct node* allocateStringNode(void* value);
struct node* allocatePersonNode(void* value);

// Custom deallocation functions
void deallocateIntNode(struct node* node);
void deallocateFloatNode(struct node* node);
void deallocateDoubleNode(struct node* node);
void deallocateStringNode(struct node* node);
void deallocatePersonNode(struct node* node);

// Custom print functions
void printSpacer();
void printInts(void* a);
void printFloats(void* a);
void printDoubles(void* a);
void printStrings(void* a);
void printPersons(void* a);


#endif

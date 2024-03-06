/*  Group No : 7
2020B1A71605P Samarth Trivedi
2020B1A71612P Harshil Sethi
2020B1A70644P Nitish Sahani
2020B3A70786P Neel Dedhia
2020B3A70794P Harsh Rathi
2020B5A70604P Divyan Poddar
*/

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

// Struct for the variable
typedef struct
{
    char name[30];
    int varNum;
    bool isTerminal;
} variable;

// Node structure for the linked list
typedef struct Node
{
    variable *data;
    struct Node *next;
} Node;

// Struct for the stack
typedef struct
{
    Node *top;
} Stack;

// Function prototypes
Node *createNode(variable *data);
Stack *initializeStack();
bool isEmpty(Stack *stack);
void push(Stack *stack, variable *data);
variable *pop(Stack *stack);

#endif /* STACK_H */

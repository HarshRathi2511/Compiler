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

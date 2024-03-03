#include "stack_ADT.h"
#include <stdio.h>
#include <stdlib.h>

// Function to create a new node
Node *createNode(variable *data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to initialize the stack
Stack *initializeStack()
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (stack == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    stack->top = NULL;
    return stack;
}

// Function to check if the stack is empty
bool isEmpty(Stack *stack)
{
    return stack->top == NULL;
}

// Function to push an element onto the stack
void push(Stack *stack, variable *data)
{
    Node *newNode = createNode(data);
    newNode->next = stack->top;
    stack->top = newNode;
}

// Function to pop an element from the stack
variable *pop(Stack *stack)
{
    if (isEmpty(stack))
    {
        printf("Stack underflow!\n");
        exit(EXIT_FAILURE);
    }
    Node *temp = stack->top;
    variable *data = temp->data;
    stack->top = temp->next;
    free(temp);
    return data;
}

// int main()
// {
//     // Example usage
//     Stack *stack = initializeStack();

//     // Creating some variable objects
//     variable var1 = {.varNum = 1, .isTerminal = false};
//     strcpy(var1.name, "variable 1");
//     variable var2 = {.varNum = 2, .isTerminal = true};
//     strcpy(var2.name, "variable 2");
//     variable var3 = {.varNum = 3, .isTerminal = false};
//     strcpy(var3.name, "variable 3");

//     // Pushing variables onto the stack
//     push(stack, &var1);
//     push(stack, &var2);
//     push(stack, &var3);

//     // Getting the top variable
//     printf("Top variable: %s\n", top(stack)->name);

//     // Popping and printing variables until stack is empty
//     printf("Popping variables:\n");
//     while (!isEmpty(stack))
//     {
//         variable *var = pop(stack);
//         printf("%s\n", var->name);
//     }

//     // Freeing the memory allocated for the stack
//     destroyStack(stack);

//     return 0;
// }

/*  Group No : 7
2020B1A71605P Samarth Trivedi
2020B1A71612P Harshil Sethi
2020B1A70644P Nitish Sahani
2020B3A70786P Neel Dedhia
2020B3A70794P Harsh Rathi
2020B5A70604P Divyan Poddar
*/


#ifndef MY_HEADER_FILE_H
#define MY_HEADER_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // Include the header file for bool type
#include <sys/types.h>
#include "tree_ADT.h"
#include "stack_ADT.h"
#define NON_TERMINALS 53
#define TERMINALS 58 // 56 terminals + 1 epsilon + 1 dollar

//#include <time.h>

// Define the Token struct
// typedef struct
// {
//     char name[30];
//     int varNum;
//     bool isTerminal;
// } variable;

typedef struct followADT
{
    variable *v;
    struct followADT *next;
    bool first;
} followADT;

// Define the typedef for struct varNode
typedef struct varNode
{
    variable *v;
    struct varNode *next;
} varNode;

typedef struct token_input
{
    int linenum;
    char name[30];
    int varNum;
    char value[100];
    struct token_input *next_token;
} token_input;

// Define the typedef for struct rule
typedef struct rule
{
    struct rule *nextRule;
    varNode *headNode;
} rule;
#define NON_TERMINALS 53
#define TERMINALS 58 // 56 terminals + 1 epsilon + 1 dollar

#endif /* MY_HEADER_FILE_H */

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

#include <time.h>

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

// Function prototypes
void initializeStackandTree();
void freeAllafterMenu();
void freeGrammar();
void freeAllafterParsing();
void printparsingtree();
unsigned long long recFirst(int i);
bool inFirst(variable *nonTerminal, variable *terminal);
void populateFirst();
unsigned long long recFollow(int i);
void removeEpsilon();
void fillMatrix();
void updateMatrix(rule *currRule, unsigned long long mask, int ind);
void printMatrix();
void exportToCSV();
void populateFollow();
void loopFollow();
void test_follow();
void initializeErrorTokens();
token_input *create_Token_input(int linenum, char name[30], int varNum, token_input *next_token);
char *ruleToString(int i, rule *currRule);
void printRule(int i, rule *currRule);
void fill_epsilon();
void print_epsilon();
bool inFollow(variable *nonTerminal, variable *terminal);
void populateTerminals(const char *filename);
void populateNonTerminals(const char *filename);
void printTerminals();
void printNonTerminals();
void readGrammar(const char *filename);
void printGrammar();
void parser(token_input *token);
followADT *createfADT(variable *v, followADT *next, bool first);
void test_first();
void epsilonFirst();
int assignNumToTokens(token_input *head);

#endif /* MY_HEADER_FILE_H */

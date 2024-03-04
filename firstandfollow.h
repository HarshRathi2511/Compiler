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

// Global variables
rule *matrix[NON_TERMINALS][TERMINALS];
variable *terminal_array[TERMINALS];
variable *non_terminal_array[NON_TERMINALS];
varNode First[NON_TERMINALS]; // Declare an array of varNode
rule *grammar[NON_TERMINALS];
bool epsilon[NON_TERMINALS];
unsigned long long first[NON_TERMINALS];
unsigned long long follow[NON_TERMINALS];
token_input *parser_input_head;
followADT *headChain[NON_TERMINALS];
followADT *prevChain[NON_TERMINALS];
rule *ERROR;
rule *SYN;
variable *ERROR_var;
variable *SYN_var;

// Function prototypes
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
TreeNode *parser(token_input *token);
followADT *createfADT(variable *v, followADT *next, bool first);
void test_first();
void epsilonFirst();
void assignNumToTokens(token_input *head);

#endif /* MY_HEADER_FILE_H */

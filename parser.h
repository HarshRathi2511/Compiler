/*  Group No : 7
2020B1A71605P Samarth Trivedi
2020B1A71612P Harshil Sethi
2020B1A70644P Nitish Sahani
2020B3A70786P Neel Dedhia
2020B3A70794P Harsh Rathi
2020B5A70604P Divyan Poddar
*/

#ifndef MY_HEADER_FILE
#define MY_HEADER_FILE
#include "parserDef.h"
#include "tree_ADT.h"
#include "stack_ADT.h"


// Function prototypes
void initializeStackandTree();
void freeAllafterMenu();
void freeGrammar();
void freeAllafterParsing();
void printparsingtree(FILE *file);
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

#endif /* MY_HEADER_FILE */

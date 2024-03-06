/*  Group No : 7
2020B1A71605P Samarth Trivedi
2020B1A71612P Harshil Sethi
2020B1A70644P Nitish Sahani
2020B3A70786P Neel Dedhia
2020B3A70794P Harsh Rathi
2020B5A70604P Divyan Poddar
*/

#ifndef TREE_H
#define TREE_H

#include <stdbool.h>
#include <string.h>
#include "stack_ADT.h"

typedef struct TreeNode
{
    variable *data;
    struct TreeNode *firstChild;
    struct TreeNode *nextSibling;
    struct TreeNode *parent;
    int line_no;
    char lex[30];
} TreeNode;

TreeNode *createTreeNode(variable *data);
void printTreeToFile(FILE *file, TreeNode *node);
void addChild(TreeNode *parent, TreeNode *child);
void freeTreeNode(TreeNode *node);
variable *createvariable(const char *name, int varNum, bool isTerminal);

#endif /* TREE_H */

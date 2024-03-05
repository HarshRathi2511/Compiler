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

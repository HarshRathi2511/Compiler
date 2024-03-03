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
} TreeNode;

TreeNode *createTreeNode(variable *data);
void addChild(TreeNode *parent, TreeNode *child);
void printTree(TreeNode *root, int depth);
void freeTreeNode(TreeNode *node);
variable *createvariable(const char *name, int varNum, bool isTerminal);

#endif /* TREE_H */

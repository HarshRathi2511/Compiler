#ifndef TREE_H
#define TREE_H

#include <stdbool.h>

typedef struct Variable
{
    char name[30];
    int varNum;
    bool isTerminal;
} Variable;

typedef struct TreeNode
{
    Variable *data;
    struct TreeNode *firstChild;
    struct TreeNode *nextSibling;
    struct TreeNode *parent;
} TreeNode;

TreeNode *createTreeNode(Variable *data);
void addChild(TreeNode *parent, TreeNode *child);
void printTree(TreeNode *root, int depth);
void freeTreeNode(TreeNode *node);
Variable *createVariable(const char *name, int varNum, bool isTerminal);

#endif /* TREE_H */

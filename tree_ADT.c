#include <stdio.h>
#include <stdlib.h>
#include "tree_ADT.h"

TreeNode *createTreeNode(variable *data)
{
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if (node)
    {
        node->data = data;
        node->firstChild = NULL;
        node->nextSibling = NULL;
        node->parent = NULL;
    }
    return node;
}

void addChild(TreeNode *PAR, TreeNode *CHILD)
{
    if (PAR == NULL || CHILD == NULL)
        return;
    if (PAR->firstChild == NULL)
    {
        PAR->firstChild = CHILD;
        CHILD->parent = PAR;
    }
    else
    {
        TreeNode *sibling = PAR->firstChild;
        while (sibling->nextSibling != NULL)
            sibling = sibling->nextSibling;
        sibling->nextSibling = CHILD;
        CHILD->parent = PAR;
    }
}

// void printTree(TreeNode *root, int depth)
// {
//     if (root == NULL)
//         return;
//     for (int i = 0; i < depth; ++i)
//         printf("  ");
//     printf("%s (VarNum: %d, IsTerminal: %s)\n", root->data->name, root->data->varNum, root->data->isTerminal ? "true" : "false");
//     printTree(root->firstChild, depth + 1);
//     printTree(root->nextSibling, depth);
// }
void printSpaces(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf(" ");
    }
}

void printTree(TreeNode *root, int depth)
{
    if (root == NULL)
        return;

    // Print the current node
    printSpaces(depth * 4);
    if (root->data->isTerminal)
    {
        printf("%s", root->data->name);
    }
    else
    {
        printf("%s\n", root->data->name);
    }

    // Recursively print the children
    TreeNode *child = root->firstChild;
    while (child != NULL)
    {
        printTree(child, depth + 1);
        child = child->nextSibling;
    }
}

void freeTreeNode(TreeNode *node)
{
    if (node == NULL)
        return;
    freeTreeNode(node->firstChild);
    freeTreeNode(node->nextSibling);
    free(node);
}
variable *createvariable(const char *name, int varNum, bool isTerminal)
{
    variable *var = (variable *)malloc(sizeof(variable));
    if (var)
    {
        strncpy(var->name, name, sizeof(var->name) - 1);
        var->name[sizeof(var->name) - 1] = '\0'; // Ensure null-termination
        var->varNum = varNum;
        var->isTerminal = isTerminal;
    }
    return var;
}

// int main()
// {
//     // Creating some sample variables
//     variable *var1 = (variable *)malloc(sizeof(variable));
//     strcpy(var1->name, "variable 1");
//     var1->varNum = 1;
//     var1->isTerminal = true;

//     variable *var2 = (variable *)malloc(sizeof(variable));
//     strcpy(var2->name, "variable 2");
//     var2->varNum = 2;
//     var2->isTerminal = false;

//     variable *var3 = (variable *)malloc(sizeof(variable));
//     strcpy(var3->name, "variable 3");
//     var3->varNum = 3;
//     var3->isTerminal = true;

//     // Creating tree nodes
//     TreeNode *root = createTreeNode(var1);
//     TreeNode *child1 = createTreeNode(var2);
//     TreeNode *child2 = createTreeNode(var3);

//     // Building the tree structure
//     addChild(root, child1);
//     addChild(root, child2);

//     // Printing the tree
//     printf("Tree Structure:\n");
//     printTree(root, 0);

//     // Freeing allocated memory
//     freeTreeNode(root);

//     return 0;
// }

// int main()
// {
//     // Create variable nodes
//     variable *rootVar = createvariable("Root", 0, false);
//     variable *child1Var = createvariable("Child1", 1, false);
//     variable *child2Var = createvariable("Child2", 2, false);
//     variable *child3Var = createvariable("Child3", 3, false);
//     variable *child4Var = createvariable("Child4", 4, false);
//     variable *child5Var = createvariable("Child5", 5, false);
//     variable *child6Var = createvariable("Child6", 6, false);
//     variable *child7Var = createvariable("Child7", 7, false);
//     variable *child8Var = createvariable("Child8", 8, false);
//     variable *child9Var = createvariable("Child9", 9, false);
//     variable *child10Var = createvariable("Child10", 10, false);

//     // Create tree nodes
//     TreeNode *rootNode = createTreeNode(rootVar);
//     TreeNode *child1Node = createTreeNode(child1Var);
//     TreeNode *child2Node = createTreeNode(child2Var);
//     TreeNode *child3Node = createTreeNode(child3Var);
//     TreeNode *child4Node = createTreeNode(child4Var);
//     TreeNode *child5Node = createTreeNode(child5Var);
//     TreeNode *child6Node = createTreeNode(child6Var);
//     TreeNode *child7Node = createTreeNode(child7Var);
//     TreeNode *child8Node = createTreeNode(child8Var);
//     TreeNode *child9Node = createTreeNode(child9Var);
//     TreeNode *child10Node = createTreeNode(child10Var);

//     // Build the tree structure
//     addChild(rootNode, child1Node);
//     addChild(rootNode, child2Node);
//     addChild(child1Node, child3Node);
//     addChild(child1Node, child4Node);
//     addChild(child2Node, child5Node);
//     addChild(child2Node, child6Node);
//     addChild(child3Node, child7Node);
//     addChild(child3Node, child8Node);
//     addChild(child4Node, child9Node);
//     addChild(child4Node, child10Node);

//     // Print the tree
//     printf("Sample Tree:\n");
//     printTree(rootNode, 0);

//     // Free memory
//     freeTreeNode(rootNode);

//     return 0;
// }
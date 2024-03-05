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
        node->line_no = -1;
        strcpy(node->lex, "");
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

void printSpaces(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("--");
    }
}
// void printTree(TreeNode *root, int depth)
// {
//     if (root == NULL)
//         return;

//     // Print the current node
//     printSpaces(depth * 2);
//     if (depth > 0)
//         printf("     ");
//     printf("%s\n", root->data->name);

//     // Recursively print the children
//     TreeNode *child = root->firstChild;
//     while (child != NULL)
//     {
//         printTree(child, depth + 1);
//         child = child->nextSibling;
//     }
// }

void printTn(TreeNode *par)
{
    if (!par)
        return;
    bool isleafnode = (par->firstChild == NULL);
    char lexeme_print[30];
    char token_name[30];

    if (isleafnode)
    {
        if (par->lex)
            strcpy(lexeme_print, par->lex);
        else
            strcpy(lexeme_print, "----");
        strcpy(token_name, par->data->name);
    }
    else
    {
        strcpy(lexeme_print, "----");
        strcpy(token_name, "----");
    }
    if (!strcmp(token_name, "epsilon"))
    {
        // do nothing in this case
    }
    else
    {
        char valif[30];
        strcpy(valif, "----");
        if (!strcmp(token_name, "TK_NUM") || !strcmp(token_name, "TK_RNUM"))
        {
            strcpy(valif, lexeme_print);
            strcpy(lexeme_print, "----");
        }
        printf("%-22s %-22d %-22s %-22s %-27s %-22s %-22s\n",
               lexeme_print,
               par->line_no,
               token_name,
               valif,
               (par->parent ? par->parent->data->name : "----"),
               ((par->firstChild == NULL ? "yes" : "no")),
               (par->data->name));
    }
}
void printTreeInorder(TreeNode *node, TreeNode *par, int depth)
{

    if (!node)
    {
        printTn(par);
        return;
    }

    printTreeInorder(node->firstChild, node, depth + 1);

    if (node->nextSibling == NULL)
    {

        if (par && par->firstChild == node)
        {
            printTn(par);
        }
        return;
    }
    else if (node->nextSibling->nextSibling == NULL)
    {
        printTn(par);

        printTreeInorder(node->nextSibling, par, depth);
    }
    else
    {
        printTreeInorder(node->nextSibling, par, depth);
    }
}

void printTree(TreeNode *node)
{
    printf("%-22s %-22s %-22s %-22s %-27s %-22s %-22s\n", "lexeme", "lineno", "tokenName", "valueIfNumber", "parentNodeSymbol", "isLeafNode(yes/no)", "nodeSymbol");
    printTreeInorder(node, NULL, 0);
}

// void printTreeInorder(TreeNode *node, int depth)
// {
//     if (node == NULL)
//         return;

//     printTreeInorder(node->firstChild, depth + 1);
//     bool isleafnode = (node->firstChild == NULL);
//     char lexeme_print[30];
//     char token_name[30];

//     if (isleafnode)
//     {
//         if (node->lex)
//             strcpy(lexeme_print, node->lex);
//         else
//             strcpy(lexeme_print, "----");
//         strcpy(token_name, node->data->name);
//     }
//     else
//     {
//         strcpy(lexeme_print, "----");
//         strcpy(token_name, "----");
//     }

//     printf("%-30s %-20d %-30s %-30s %-30s %-10d\n", lexeme_print, node->line_no,
//            token_name,
//            ("xyz"),
//            (node->parent ? node->parent->data->name : "----"),
//            ((node->firstChild == NULL ? "yes" : "no")),
//            (node->data->name));

//     printTreeInorder(node->nextSibling, depth);
// }

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
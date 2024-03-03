#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // Include the header file for bool type
#include <sys/types.h>
#define NON_TERMINALS 53
#define TERMINALS 57 // 56 terminals + 1 epsilon
// Define the Token struct
// Define the typedef for struct variable
typedef struct
{
    char name[30];
    int varNum;
    bool isTerminal;
} variable;

// Define the typedef for struct varNode
typedef struct varNode
{
    variable *v;
    struct varNode *next;
} varNode;

// Define the typedef for struct rule
typedef struct rule
{
    struct rule *nextRule;
    varNode *headNode;
} rule;

variable *terminal_array[TERMINALS];
variable *non_terminal_array[NON_TERMINALS];
varNode First[NON_TERMINALS]; // Declare an array of varNode
rule *grammar[NON_TERMINALS];

// Function to populate terminals array from file
void populateTerminals(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", filename);
        return;
    }

    char buffer[1024];
    int varNum = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        char *token = strtok(buffer, ",");
        while (token != NULL)
        {
            terminal_array[varNum] = (variable *)malloc(sizeof(variable));
            strcpy(terminal_array[varNum]->name, token);
            terminal_array[varNum]->varNum = varNum;
            terminal_array[varNum]->isTerminal = true;
            varNum++;
            token = strtok(NULL, ",");
        }
    }

    fclose(file);
}

// Function to populate non-terminals array from file
void populateNonTerminals(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", filename);
        return;
    }

    char buffer[1024];
    int varNum = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        char *token = strtok(buffer, ",");
        while (token != NULL)
        {
            non_terminal_array[varNum] = (variable *)malloc(sizeof(variable));
            strcpy(non_terminal_array[varNum]->name, token);
            non_terminal_array[varNum]->varNum = varNum;
            non_terminal_array[varNum]->isTerminal = false;
            varNum++;
            token = strtok(NULL, ",");
        }
    }

    fclose(file);
}

// Function to print terminals
void printTerminals()
{
    printf("Terminals:\n");
    for (int i = 0; i < TERMINALS; i++)
    {
        printf("%d.%s %zu\n", terminal_array[i]->varNum, terminal_array[i]->name, strlen(terminal_array[i]->name));
    }
    printf("\n");
}

// Function to print non-terminals
void printNonTerminals()
{
    printf("Non-Terminals:\n");
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        printf("%d.%s %zu\n", non_terminal_array[i]->varNum, non_terminal_array[i]->name, strlen(non_terminal_array[i]->name));
    }
    printf("\n");
}

// Function to read grammar rules from file and organize them into a linked list
void readGrammar(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", filename);
        return;
    }
    int lineno = 0;
    char buffer[10240];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        lineno++;
        // char *token = strtok(buffer, " ");
        char LHS[100];
        char RHS[100];
        char RHS_array[30][30];
        printf("**************************************************** \n");
        printf("%d.%s\n", lineno, buffer);
        sscanf(buffer, "%s ===> %[^\n\t]", LHS, RHS);
        printf("LHS is %s size = %zu\n", LHS, strlen(LHS));
        // printf("RHS is %s \n", RHS);

        char *token = strtok(RHS, " ");
        int RHS_index = 0;
        while (token != NULL)
        {
            // printf("%s\n",token);
            strcpy(RHS_array[RHS_index], token);
            token = strtok(NULL, " ");
            RHS_index++;
        }
        printf("RHS INDEX IS %d\n", RHS_index);

        for (int i = 0; i < RHS_index; i++)
        {
            printf(".%s%zu \t", RHS_array[i], strlen(RHS_array[i]));
        }

        int lhsIndex = -1;
        for (int i = 0; i < NON_TERMINALS; i++)
        {
            if (strcmp(non_terminal_array[i]->name, LHS) == 0)
            {
                lhsIndex = i;
                printf("LHS Index of %s is %d. \n", LHS, lhsIndex);
                break;
            }
        }

        // If non-terminal not found, skip this rule
        if (lhsIndex == -1)
        {
            printf("Non-terminal %s not found.\n", LHS);
            continue;
        }
        printf("\n**************************************************** \n");
        // Create a new rule node
        rule *newRule = (rule *)malloc(sizeof(rule));
        newRule->nextRule = NULL;
        newRule->headNode = NULL;

        for (int i = 0; i < RHS_index; i++)
        {
            varNode *newNode = (varNode *)malloc(sizeof(varNode));

            // Determine if the variable is terminal or non-terminal
            bool isTerminal = RHS_array[i][0] != '<'; // If it doesn't start with '<', it's a terminal
            if (RHS_array[i][0] == 'e')
                isTerminal = true;
            // Find the corresponding variable in the arrays
            variable *var;
            if (isTerminal)
            {
                bool fl1 = true;
                for (int j = 0; j < TERMINALS; j++)
                {
                    if (strcmp(terminal_array[j]->name, RHS_array[i]) == 0)
                    {
                        var = terminal_array[j];
                        fl1 = false;
                        break;
                    }
                }
                if (fl1)
                    printf("NOT FOUND  terminal for %d %d\n", lhsIndex, i);
            }
            else
            {
                bool fl2 = true;
                for (int j = 0; j < NON_TERMINALS; j++)
                {

                    if (strcmp(non_terminal_array[j]->name, RHS_array[i]) == 0)
                    {
                        var = non_terminal_array[j];
                        fl2 = false;
                        break;
                    }
                }
                if (fl2)
                    printf("NOT FOUND non terminal for %d %d\n", lhsIndex, i);
            }

            // Populate the new node
            newNode->v = var;
            newNode->next = NULL;

            // Append the new node to the rule's linked list of variables
            if (newRule->headNode == NULL)
            {
                newRule->headNode = newNode;
            }
            else
            {
                varNode *current = newRule->headNode;
                while (current->next != NULL)
                {
                    current = current->next;
                }
                current->next = newNode;
            }
        }

        // Append the new rule to the array of rules for the corresponding non-terminal
        if (grammar[lhsIndex] == NULL)
        {
            grammar[lhsIndex] = newRule;
        }
        else
        {
            rule *current = grammar[lhsIndex];
            while (current->nextRule != NULL)
            {
                current = current->nextRule;
            }
            current->nextRule = newRule;
        }
    }

    fclose(file);
}

// Function to print the grammar rules
void printGrammar()
{
    int j = 1;
    printf("Grammar Rules:\n");
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        rule *r = grammar[i];
        if (grammar[i] != NULL)
        {
            printf("%d.%s ===> ", j, non_terminal_array[i]->name);
            rule *currentRule = grammar[i];
            while (currentRule != NULL)
            {
                varNode *currentNode = currentRule->headNode;
                while (currentNode != NULL)
                {
                    printf("%s ", currentNode->v->name);
                    currentNode = currentNode->next;
                }
                if (currentRule->nextRule != NULL)
                {
                    printf("| ");
                }
                currentRule = currentRule->nextRule;
            }
            printf("\n");
            // r = r->nextRule;
            j++;
        }
    }
    printf("\n");
}

int main()
{
    populateTerminals("terminals.txt");
    populateNonTerminals("non_terminals.txt");
    // test();
    // printTerminals();
    // printNonTerminals();
    readGrammar("grammar.txt");
    printGrammar();

    // Free dynamically allocated memory
    for (int i = 0; i < TERMINALS; i++)
    {
        free(terminal_array[i]);
    }
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        free(non_terminal_array[i]);
    }

    return 0;
}

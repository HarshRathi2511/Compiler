#include "firstandfollow.h"

void initializeErrorTokens()
{
    ERROR_var = (variable *)malloc(sizeof(variable));
    SYN_var = (variable *)malloc(sizeof(variable));
    strcpy(ERROR_var->name, "ERROR");
    strcpy(SYN_var->name, "SYN");
    ERROR = (rule *)malloc(sizeof(rule));
    SYN = (rule *)malloc(sizeof(rule));
    ERROR->headNode = (varNode *)malloc(sizeof(varNode));
    SYN->headNode = (varNode *)malloc(sizeof(varNode));
    ERROR->headNode->v = ERROR_var;
    SYN->headNode->v = SYN_var;
}

token_input *create_Token_input(int linenum, char name[30], int varNum, token_input *next_token)
{
    token_input *ptr = (token_input *)malloc(sizeof(token_input));
    ptr->linenum = linenum;
    strcpy(ptr->name, name);
    ptr->varNum = varNum;
    ptr->next_token = next_token;
    return ptr;
}

// Function to populate terminals array from file
char *ruleToString(int i, rule *currRule)
{
    int MAX_STRING_LENGTH = 1000;
    static char output[1000]; // Static array for fixed size

    if (!currRule)
    {
        snprintf(output, MAX_STRING_LENGTH, "null");
        return output;
    }

    if (currRule == ERROR)
    {
        strcpy(output, "ERROR");
        return output;
    }

    if (currRule == SYN)
    {
        strcpy(output, "SYN");
        return output;
    }

    // Initialize output string
    output[0] = '\0';

    // Concatenate non-terminal name
    snprintf(output + strlen(output), MAX_STRING_LENGTH - strlen(output), "%s -> ", non_terminal_array[i]->name);

    // Concatenate variable names
    varNode *head = currRule->headNode;
    while (head)
    {
        snprintf(output + strlen(output), MAX_STRING_LENGTH - strlen(output), "%s ", head->v->name);
        head = head->next;
    }

    return output;
}
void printRule(int i, rule *currRule)
{
    if (!currRule)
    {
        printf("null");
        return;
    }
    printf("%s -> ", non_terminal_array[i]->name);
    varNode *head = currRule->headNode;
    while (head)
    {
        printf("%s", head->v->name);
        head = head->next;
    }
}
void fillMatrix()
{
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        for (int j = 0; j < TERMINALS; j++)
        {
            matrix[i][j] = ERROR;
        }
    }

    for (int i = 0; i < NON_TERMINALS; i++)
    {
        rule *curr = grammar[i];
        while (curr != NULL)
        {
            varNode *head = curr->headNode;
            unsigned long long intersection = 0;
            while (head && !head->v->isTerminal && epsilon[head->v->varNum])
            {
                intersection = first[i] & first[head->v->varNum];
                updateMatrix(curr, intersection, i);
                head = head->next;
            }
            if (!head)
            {
                curr = curr->nextRule;
                continue;
            }
            else if (head->v->isTerminal)
            {
                if (strcmp(head->v->name, "epsilon") == 0)
                {
                    updateMatrix(curr, follow[i], i);
                }
                intersection = first[i] & (1ULL << head->v->varNum);
            }
            else if (!head->v->isTerminal)
            {
                intersection = first[i] & first[head->v->varNum];
            }
            updateMatrix(curr, intersection, i);
            curr = curr->nextRule;
        }
        updateMatrix(SYN, follow[i], i);
    }
}

followADT *createfADT(variable *v, followADT *next, bool first)
{
    followADT *ptr = (followADT *)malloc(sizeof(followADT));
    ptr->first = first;
    ptr->v = v;
    ptr->next = next;
    return ptr;
}

void updateMatrix(rule *currRule, unsigned long long mask, int ind)
{
    for (int i = 0; i < TERMINALS; i++)
    {
        if (mask & ((1ULL << i)))
        {
            if (currRule == SYN && matrix[ind][i] != ERROR)
                continue;
            matrix[ind][i] = currRule;
        }
    }
}

void printMatrix()
{
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        for (int j = 0; j < TERMINALS; j++)
        {
            printRule(i, matrix[i][j]);
            printf("\t");
        }
        printf("\n");
    }
}
void exportToCSV()
{
    const char *filename = "matrix.csv";
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    int rows = NON_TERMINALS;
    int cols = TERMINALS - 1;

    // Write column names in the first row
    fprintf(fp, ",");
    for (int j = 0; j < cols; j++)
    {
        fprintf(fp, "%s", terminal_array[j]->name);
        if (j < cols - 1)
        {
            fprintf(fp, ",");
        }
    }
    fprintf(fp, "\n");

    // Write the matrix to the CSV file
    for (int i = 0; i < rows; i++)
    {
        // Write row name in the first column
        fprintf(fp, "%s,", non_terminal_array[i]->name);

        for (int j = 0; j < cols; j++)
        {
            char *st = ruleToString(i, matrix[i][j]);
            fprintf(fp, "%s", st);
            if (j < cols - 1)
            {
                fprintf(fp, ",");
            }
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    printf("Matrix exported to %s successfully.\n", filename);
}
void populateFollow()
{
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        follow[i] = 0;
        headChain[i] = prevChain[i] = createfADT(non_terminal_array[i], NULL, false);
    }

    followADT *start = createfADT(terminal_array[0], NULL, true);
    prevChain[0]->next = start;
    prevChain[0] = prevChain[0]->next;

    for (int i = 0; i < NON_TERMINALS; i++)
    {
        rule *currRule = grammar[i];
        while (currRule != NULL)
        {
            varNode *ptr = currRule->headNode;
            while (ptr != NULL)
            {
                varNode *ptrNext = ptr->next;
                if (ptr->v->isTerminal)
                {
                    ptr = ptr->next;
                    continue;
                }
                followADT *curr;
                while (ptrNext != NULL && !ptrNext->v->isTerminal && epsilon[ptrNext->v->varNum])
                {
                    curr = createfADT(ptrNext->v, NULL, true);
                    prevChain[ptr->v->varNum]->next = curr;
                    prevChain[ptr->v->varNum] = prevChain[ptr->v->varNum]->next;
                    ptrNext = ptrNext->next;
                }
                if (ptrNext == NULL)
                {
                    curr = createfADT(non_terminal_array[i], NULL, false);
                }
                else if (ptrNext->v->isTerminal)
                {
                    curr = createfADT(ptrNext->v, NULL, true);
                }
                else if (!ptrNext->v->isTerminal)
                {
                    curr = createfADT(ptrNext->v, NULL, true);
                }

                prevChain[ptr->v->varNum]->next = curr;
                prevChain[ptr->v->varNum] = prevChain[ptr->v->varNum]->next;
                ptr = ptr->next;
            }
            currRule = currRule->nextRule;
        }
    }

    // for (int i = 0; i < NON_TERMINALS; i++)
    // {
    //     followADT *ptr = headChain[i];
    //     printf("%s --> ", non_terminal_array[i]->name);
    //     while (ptr != NULL)
    //     {
    //         printf("%s,%d \t", ptr->v->name, ptr->first);
    //         ptr = ptr->next;
    //     }
    //     printf("\n");
    // }

    // for (int i = 0; i < NON_TERMINALS; i++)
    // {
    //     printf("%d ",i);
    //     if (follow[i] == 0)
    //         follow[i] = recFollow(i);

    // }
    printf("read_grammar successfully worked\n!");
    loopFollow();
    printf("read_grammar successfully worked\n!");
    removeEpsilon();
}
void removeEpsilon()
{
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        unsigned long long mask = 1ULL << (TERMINALS - 1);
        follow[i] = follow[i] & (~mask);
    }
}
void loopFollow()
{
    bool noChange = false;
    while (!noChange)
    {
        noChange = true;
        for (int i = 0; i < NON_TERMINALS; i++)
        {
            followADT *curr = headChain[i];
            while (curr != NULL)
            {
                if (curr->first)
                {
                    if (curr->v->isTerminal && (follow[i] != (follow[i] | (1ULL << curr->v->varNum))))
                    {
                        follow[i] = follow[i] | (1ULL << curr->v->varNum);
                        noChange = false;
                    }
                    else if (!curr->v->isTerminal && (follow[i] != (follow[i] | first[curr->v->varNum])))
                    {
                        follow[i] = follow[i] | first[curr->v->varNum];
                        noChange = false;
                    }
                }
                else
                {
                    if (follow[i] != (follow[i] | follow[curr->v->varNum]))
                    {
                        follow[i] = follow[i] | follow[curr->v->varNum];
                        noChange = false;
                    }
                }
                curr = curr->next;
            }
        }
    }
}
unsigned long long recFollow(int i)
{
    unsigned long long ans = 0;
    followADT *currADT = headChain[i];
    if (follow[i] != 0)
        return follow[i];
    while (currADT != NULL)
    {
        if (currADT->first)
        {
            if (currADT->v->isTerminal)
            {
                ans = ans | (1ULL << currADT->v->varNum);
            }
            else
                ans = ans | first[currADT->v->varNum];
        }
        else
        {
            if (currADT->v->varNum == i)
            {
                currADT = currADT->next;
                continue;
            }
            else
                ans = ans | recFollow(currADT->v->varNum);
        }
        currADT = currADT->next;
    }
    return follow[i] = ans;
}

bool inFollow(variable *nonTerminal, variable *terminal)
{
    unsigned long long num = follow[nonTerminal->varNum];
    int ind = terminal->varNum;
    unsigned long long mask = 1ULL << ind;
    return (num & mask) != 0;
}
void test_follow()
{
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        printf("%s -> ", non_terminal_array[i]->name);
        for (int j = 0; j < TERMINALS; j++)
        {
            if (inFollow(non_terminal_array[i], terminal_array[j]))
            {
                printf("%s \t", terminal_array[j]->name);
            }
        }
        printf("\n");
    }
}

void fill_epsilon()
{
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        epsilon[i] = false;
    }
}

void print_epsilon()
{
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        printf("%s %d \n", non_terminal_array[i]->name, epsilon[i]);
    }
}

bool inFirst(variable *nonTerminal, variable *terminal)
{
    unsigned long long num = first[nonTerminal->varNum];
    int ind = terminal->varNum;
    // if(ind==TERMINALS-1 && !epsilon[nonTerminal->varNum]) return false;
    unsigned long long mask = 1ULL << ind;
    return (num & mask) != 0;
}

void populateFirst()
{
    for (int i = 0; i < NON_TERMINALS; i++)
        first[i] = 0;
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        first[i] = recFirst(i);
    }
}
unsigned long long recFirst(int i)
{
    unsigned long long ans = 0;
    rule *production = grammar[i];
    if (first[i] != 0)
        return first[i];
    while (production != NULL)
    {
        varNode *head = production->headNode;
        while (head != NULL && !head->v->isTerminal && epsilon[head->v->varNum] == true)
        {
            ans = ans | recFirst(head->v->varNum);
            head = head->next;
        }
        if (head == NULL)
        {
        }
        else if (!head->v->isTerminal)
        {
            ans = ans | recFirst(head->v->varNum);
        }
        else if (head->v->isTerminal)
        {
            ans = ans | (1ULL << head->v->varNum);
        }

        production = production->nextRule;
    }
    // printf("some random iteration ");
    return first[i] = ans;
}

void test_first()
{
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        printf("%s -> ", non_terminal_array[i]->name);
        for (int j = 0; j < TERMINALS; j++)
        {
            if (inFirst(non_terminal_array[i], terminal_array[j]))
            {
                printf("%s \t", terminal_array[j]->name);
            }
        }
        printf("\n");
    }
}

// Function to populate terminals array from file

void epsilonFirst()
{
    unsigned long long mask = 1ULL << (TERMINALS - 1);
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        if (!epsilon[i])
            first[i] = first[i] & ~mask;
    }
}
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
        // printf("**************************************************** \n");
        // printf("%d.%s\n", lineno, buffer);
        sscanf(buffer, "%s ===> %[^\n\t]", LHS, RHS);
        // printf("LHS is %s size = %zu\n", LHS, strlen(LHS));
        // printf("RHS is %s \n", RHS);

        int lhsIndex = -1;
        for (int i = 0; i < NON_TERMINALS; i++)
        {
            if (strcmp(non_terminal_array[i]->name, LHS) == 0)
            {
                lhsIndex = i;
                // printf("LHS Index of %s is %d. \n", LHS, lhsIndex);
                break;
            }
        }

        char *token = strtok(RHS, " ");
        int RHS_index = 0;
        while (token != NULL)
        {
            // printf("%s\n",token);
            strcpy(RHS_array[RHS_index], token);
            token = strtok(NULL, " ");
            RHS_index++;
        }
        // printf("RHS INDEX IS %d\n", RHS_index);

        for (int i = 0; i < RHS_index; i++)
        {
            // printf(".%s%zu \t", RHS_array[i], strlen(RHS_array[i]));
            if (RHS_array[i][0] == 'e')
            {
                epsilon[lhsIndex] = true;
            }
        }

        // If non-terminal not found, skip this rule
        if (lhsIndex == -1)
        {
            printf("Non-terminal %s not found.\n", LHS);
            continue;
        }
        // printf("\n**************************************************** \n");
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
                    printf("NOT FOUND terminal for %d %d\n", lhsIndex, i);
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
            j++;
        }
    }
    printf("\n");
}

int assignNumToTokens(token_input *head)
{
    int num = -1;
    if (head != NULL)
    {
        for (int i = 0; i < TERMINALS; i++)
        {
            if (!strcmp(head->name, terminal_array[i]->name))
            {
                // printf("matching %s %s \n", curr->name, terminal_array[i]->name);
                num = i;
            }
        }
    }
    return num;
}
TreeNode *returnNextNode(TreeNode *curr_child)
{
    if (curr_child->parent == NULL)
        return curr_child;
    TreeNode *curr_parent = curr_child->parent;
    if (curr_child->nextSibling == NULL)
    {
        while (curr_child && curr_parent->nextSibling == NULL)
        {
            curr_child = curr_child->parent;
            curr_parent = curr_child->parent;
        }
        // printf("%s %s curr child in return next node \n", curr_child->data->name, curr_parent->data->name);
        curr_child = curr_parent->nextSibling;
        curr_parent = curr_child->parent;
    }
    else
    {
        curr_child = curr_child->nextSibling;
        curr_parent = curr_child->parent;
    }
    return curr_child;
}

void parser(token_input *token)
{
    int i = 1;
    if (token != NULL)
    {
        // If the stack's top has epsilon
        // printf("%d \t", i);
        if (isEmpty(st))
        {
            printf("Line %d Error: Invalid token %s encountered with value %s stack top EMPTY - ERROR\n", token->linenum, token->name, token->value);
            return;
        }
        if (!isEmpty(st) && st->top->data->isTerminal && st->top->data->varNum == 57)
        {
            pop(st);
            curr_child = returnNextNode(curr_child);
        }

        // printf("%d \t", i);
        i++;
        // token_input *curr = token;

        // If stacks top has dollar
        if (st->top->data->isTerminal && st->top->data->varNum == 0)
        {
            printf("Error in parsing, no more non-terminals in stack\n");
        }
        variable *curr_var = st->top->data;
        bool curr_isTerminal = curr_var->isTerminal;
        // printf("%s %s\n", curr_var->name, token->name);
        // printf("%d\n", token->varNum);
        // printf("%s \n", matrix[curr_var->varNum][token->varNum]->headNode->v->name);
        // printf("%s \n", ERROR->headNode->v->name);
        bool is_ERROR = (!curr_isTerminal && !strcmp(matrix[curr_var->varNum][token->varNum]->headNode->v->name, ERROR->headNode->v->name));
        bool is_SYN = (!curr_isTerminal && !strcmp(matrix[curr_var->varNum][token->varNum]->headNode->v->name, SYN->headNode->v->name));
        // printf("xyz\n");

        if (curr_isTerminal && curr_var->varNum == token->varNum)
        {
            // printf("MATCHED - %s %s \n", curr_var->name, token->name);
            if (!isEmpty(st))
                pop(st);

            // printf("check segfault for tree line 824 %s %s\n", curr_child->data->name, curr_child->parent->data->name);
            // if (strcmp(token->name, "TK_END"))
            // {
            curr_child = returnNextNode(curr_child);
            // }
            // token = token->next_token;
            return;
        }
        if (curr_isTerminal || is_ERROR || is_SYN)
        {
            if (is_SYN)
            {
                pop(st);
                printf("Line %d Error: Invalid token %s encountered with value %s stack top %s SYN\n", token->linenum, token->name, token->value, curr_var->name);
                curr_child = returnNextNode(curr_child);
                parser(token);
            }
            if (is_ERROR)
            {

                // token = token->next_token;
                if (epsilon[curr_var->varNum])
                {
                    pop(st);
                    parser(token);
                }
                else
                {
                    printf("Line %d Error: Invalid token %s encountered with value %s stack top %s ERROR\n", token->linenum, token->name, token->value, curr_var->name);
                }

                TreeNode *new_child = createTreeNode(ERROR_var);
                addChild(curr_child, new_child);
            }
            if (curr_isTerminal)
            {
                printf("Line %d Error: The token %s for lexeme %s  does not match with the expected token %s\n", token->linenum, token->name, token->value, curr_var->name);
                pop(st);
                curr_child = returnNextNode(curr_child);
                parser(token);
                // token = token->next_token;
            }
            /*
            Line 10 Error: Invalid token TK_SEM encountered with value ; stack top arithmeticExpression
            Line 11 Error: The token TK_SEM for lexeme ;  does not match with the expected token TK_CL
            */
            return;
        }
        // printf("Check for segfault pre temp declaration \n");

        varNode *rule_to_push = matrix[curr_var->varNum][token->varNum]->headNode;
        Stack *temp = initializeStack();
        // printf("Check for segfault post temp declaration \n");
        int j = 0;
        // printf("check segfault for tree 852\n");
        while (rule_to_push != NULL)
        {
            push(temp, rule_to_push->v);
            // printf("%d\n", j);
            j++;
            // printf("check segfault for tree 859\n");
            TreeNode *new_child = createTreeNode(rule_to_push->v);
            // printf("check segfault for tree 861 %s\n", new_child->data->name);
            addChild(curr_child, new_child);
            // printf("check segfault for tree 863\n");
            rule_to_push = rule_to_push->next;
        }
        curr_child = curr_child->firstChild;

        // printf("Check for segfault post pushing in temp \n");
        // printf("popping out: %s\n", st->top->data->name);
        pop(st);
        while (!isEmpty(temp))
        {
            variable *v;
            v = temp->top->data;
            push(st, v);
            pop(temp);
        }
        parser(token);
    }
    return;
}

// int main()
// {
//     clock_t start_time, end_time;

//     double total_CPU_time, total_CPU_time_in_seconds;

//     start_time = clock();

//     // invoke your lexer and parser here
//     populateTerminals("terminals.txt");
//     populateNonTerminals("non_terminals.txt");
//     // test();
//     // printTerminals();
//     // printNonTerminals();
//     fill_epsilon();
//     readGrammar("grammar.txt");
//     end_time = clock();
//     // printf("read_grammar successfully worked!");
//     printGrammar();
//     // print_epsilon();
//     // Free dynamically allocated memory
//     // printf("%d \n",epsilon[0]);
//     printf("CHECK FOR SEGFAULT before populate first\n");
//     populateFirst();

//     epsilonFirst();
//     test_first();
//     // printf("read_grammar successfully pre\n!");
//     populateFollow();
//     // printf("read_grammar successfully pst\n");
//     test_follow();
//     printf("CHECK FOR SEGFAULT after TEST FOLLOW \n");
//     initializeErrorTokens();
//     printf("CHECK FOR SEGFAULT after Initialise error tokens \n");
//     fillMatrix();
//     printf("CHECK FOR SEGFAULT after fill matrix \n");
//     printf("MATRIX CHECKING %d \n", matrix[1][40] == NULL);
//     printf("reached");
//     exportToCSV();
//     // printMatrix();
//     printf("reached");

//     for (int i = 0; i < TERMINALS; i++)
//     {
//         free(terminal_array[i]);
//     }
//     for (int i = 0; i < NON_TERMINALS; i++)
//     {
//         free(non_terminal_array[i]);
//     }

//     // end_time = clock();

//     total_CPU_time = (double)(end_time - start_time);

//     total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

//     printf("Total CPU time: %.6f seconds\n", total_CPU_time_in_seconds);

//     return 0;
// }

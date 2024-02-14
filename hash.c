#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SymbolTableSize 25 // No. of keywords

typedef struct {
    char *key;
    char *value;
} HashItem;

int HashFxn(char *key) {
    int hash = 0;
    char *ptr;
    for ( ptr= key; *ptr != '\0'; ptr++) {
        hash = *ptr + 64 * hash;
    }
    return hash % SymbolTableSize;
}
HashItem *SymbolTable[SymbolTableSize] = {NULL}; // Hash Table

void InsertPair(char *key, char *value) {
    int index = HashFxn(key);
    int originalIndex = index;
    do {
        if (SymbolTable[index] == NULL) {
            // Allocate memory for the new item
            SymbolTable[index] = (HashItem *)malloc(sizeof(HashItem));
            if (SymbolTable[index] == NULL) {
                fprintf(stderr, "Memory allocation failed!\n");
                exit(EXIT_FAILURE);
            }
            // Duplicate the key string
            SymbolTable[index]->key = strdup(key);
            // Assign the value
            SymbolTable[index]->value = value;
            return;
        }
        // already exists, update the value
        if (strcmp(key, SymbolTable[index]->key) == 0) {
            SymbolTable[index]->value = value;
            return;
        }
        index = (index + 1) % SymbolTableSize;
    } while (index != originalIndex);
    printf("Symbol table full\n");
}

void PopulateSymbolTable(){
    InsertPair("else", "TK_ELSE");
    InsertPair("endrecord", "TK_ENDRECORD");
    InsertPair("record", "TK_RECORD");
    InsertPair("with", "TK_WITH");
    InsertPair("parameters", "TK_PARAMETERS");
    InsertPair("end", "TK_END");
    InsertPair("while", "TK_WHILE");
    InsertPair("union", "TK_UNION");
    InsertPair("endunion", "TK_ENDUNION");
    InsertPair("definetype", "TK_DEFINETYPE");
    InsertPair("as", "TK_AS");
    InsertPair("type", "TK_TYPE");
    InsertPair("_main", "TK_MAIN");
    InsertPair("global", "TK_GLOBAL");
    InsertPair("parameter", "TK_PARAMETER");
    InsertPair("list", "TK_LIST");
    InsertPair("input", "TK_INPUT");
    InsertPair("output", "TK_OUTPUT");
    InsertPair("int", "TK_INT");
    InsertPair("real", "TK_REAL");
    InsertPair("if", "TK_IF");
    InsertPair("then", "TK_THEN");
    InsertPair("endwhile", "TK_ENDWHILE");
    InsertPair("endif", "TK_ENDIF");
    InsertPair("read", "TK_READ");
    InsertPair("write", "TK_WRITE");
    InsertPair("return", "TK_RETURN");
    InsertPair("call", "TK_CALL");
}

char *SearchPair(char *key) {
    int index = HashFxn(key);
    int originalIndex = index;
    do {
        if (SymbolTable[index] == NULL) {
            return NULL; // Key not found
        }
        if (strcmp(SymbolTable[index]->key, key) == 0) {
            return SymbolTable[index]->value; // Key found
        }
        index = (index + 1) % SymbolTableSize;
    } while (index != originalIndex);
    return NULL; // Key not found
}

// char *SearchPair(char *key) {
//     int index = HashFxn(key);
//     int originalIndex = index;
    
//     for (int i = 0; i < SymbolTableSize; i++) {
//         if (SymbolTable[index] == NULL) {
//             return NULL; // Key not found
//         }
//         if (strcmp(SymbolTable[index]->key, key) == 0) {
//             return SymbolTable[index]->value; // Key found
//         }
//         index = (index + 1) % SymbolTableSize;
//     }
    
//     return NULL; // Key not found
// }

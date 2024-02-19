#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_SIZE 100

typedef struct Entry {
    char* key;
    char* value;
    struct Entry* next;
} Entry;

Entry* hashTable[HASH_TABLE_SIZE];

//hash function
unsigned int hash(char* key) {
    unsigned int hashValue = 0;
    for (int i = 0; key[i]; i++) {
        hashValue = 31 * hashValue + key[i];
    }
    return hashValue % HASH_TABLE_SIZE;
}

//insert
void insert(char* key, char* value) {
    unsigned int index = hash(key);

    Entry* newEntry = (Entry*)malloc(sizeof(Entry));
    newEntry->key = strdup(key);
    newEntry->value = strdup(value);
    newEntry->next = NULL;

    // Insert at beginning(if no collision), else add to list
    if (hashTable[index] == NULL) {
        hashTable[index] = newEntry;
    } else {
        // Add to the beginning of the list
        newEntry->next = hashTable[index];
        hashTable[index] = newEntry;
    }
}

// Search for an entry in the hash table
Entry* search(char* key) {
    unsigned int index = hash(key);
    Entry* current = hashTable[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void AddtoHashTable(){
    insert("else", "TK_ELSE");
    insert("endrecord", "TK_ENDRECORD");
    insert("record", "TK_RECORD");
    insert("with", "TK_WITH");
    insert("parameters", "TK_PARAMETERS");
    insert("end", "TK_END");
    insert("while", "TK_WHILE");
    insert("union", "TK_UNION");
    insert("endunion", "TK_ENDUNION");
    insert("definetype", "TK_DEFINETYPE");
    insert("as", "TK_AS");
    insert("type", "TK_TYPE");
    insert("_main", "TK_MAIN");
    insert("global", "TK_GLOBAL");
    insert("parameter", "TK_PARAMETER");
    insert("list", "TK_LIST");
    insert("input", "TK_INPUT");
    insert("output", "TK_OUTPUT");
    insert("int", "TK_INT");
    insert("real", "TK_REAL");
    insert("if", "TK_IF");
    insert("then", "TK_THEN");
    insert("endwhile", "TK_ENDWHILE");
    insert("endif", "TK_ENDIF");
    insert("read", "TK_READ");
    insert("write", "TK_WRITE");
    insert("return", "TK_RETURN");
    insert("call", "TK_CALL");
}

int main() {
    // Initialize hash table
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }

    AddtoHashTable(); 

    // Example usage
    insert("x", "int");
    insert("y", "float");

    Entry* e = search("else");
    if (e != NULL) {
        printf("Found %s of type %s\n", e->key, e->value);
    } else {
        printf("Identifier not found\n");
    }

    return 0;
}

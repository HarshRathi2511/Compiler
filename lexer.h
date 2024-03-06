/*  Group No : 7
2020B1A71605P Samarth Trivedi
2020B1A71612P Harshil Sethi
2020B1A70644P Nitish Sahani
2020B3A70786P Neel Dedhia
2020B3A70794P Harsh Rathi
2020B5A70604P Divyan Poddar
*/

#ifndef LEXER_H
#define LEXER_H

#include "lexerDef.h"

void resetStateAndLineNumber();

char *Find_Comment_Symbol(char comment, const char *readBuffer);
void Remove_Comments( char *TestCaseFile,  char *CleanTestCaseFile);
unsigned int hash(char* key);
void insert(char* key, TOKEN_TYPE value);
Entry* search(char* key);
void AddtoHashTable();

void initTwinBuffer(TwinBuffer *tb);
void bufferLoader(TwinBuffer *tb, bool flag);
void checkAndLoadBuffer(TwinBuffer *tb);
void printBufferContents(TwinBuffer *tb);
int setupLexer(TwinBuffer *tb, FILE *fp);

const char* token_type_to_string(TOKEN_TYPE token);
TOKEN_TYPE string_to_token_type(const char* str);
void printToken(const TOKEN *token);
TOKEN *getNextToken(TwinBuffer *tb);

#endif

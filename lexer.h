/*  Group No : 7
 
*/
#ifndef LEXER_H
#define LEXER_H

#include "lexerDef.h"

char *Find_Comment_Symbol( char comment, const char *readBuffer);
void Remove_Comments(const char *TestCaseFile, const char *CleanTestCaseFile);
unsigned int hash(char* key);
void insert(char* key, TOKEN_TYPE value);
Entry* search(char* key);
void AddtoHashTable();

void initTwinBuffer(TwinBuffer *tb);
void bufferLoader(TwinBuffer *tb, bool loadFirst);
void checkAndLoadBuffer(TwinBuffer *tb);
void printBufferContents(TwinBuffer *tb);
int setupLexer(TwinBuffer *tb, FILE *fp);

const char* token_type_to_string(TOKEN_TYPE token);
TOKEN_TYPE string_to_token_type(const char* str);
void printToken(const TOKEN *token);
TOKEN *getNextToken(TwinBuffer *tb);

#endif
/*  Group No : 7
   
*/

#ifndef LEXERDEF_H
#define LEXERDEF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define len 1900
#define HASH_TABLE_SIZE 100
#define BUF_SIZE 512
#define MAX_LEXEME_SIZE 512

//#define KEYWORDSIZE 26 // Size of Keywords Set
typedef enum TOKEN_TYPE {
	TK_EQ, 
	TK_NE,
	TK_NOT,
	TK_AND,
	TK_OR, 
	TK_PLUS,
	TK_MINUS,
	TK_MUL, 
	TK_DIV,
	TK_DOT,
	TK_COMMA,
	TK_COLON,
	TK_SEM,
	TK_CL, 
	TK_OP,
	TK_SQR,
	TK_SQL,
	TK_COMMENT,
	TK_GT,
	TK_GE,
	TK_LT,
	TK_LE,
	TK_ASSIGNOP,
	TK_FUNID,
	TK_FIELDID,
	TK_RUID,
	TK_ID,
	TK_NUM,
	TK_RNUM,
	TK_ERROR,
	TK_ELSE,
    TK_ENDRECORD,
    TK_RECORD,
    TK_WITH,
    TK_PARAMETERS,
    TK_END,
    TK_WHILE,
    TK_UNION,
    TK_ENDUNION,
    TK_DEFINETYPE,
    TK_AS,
    TK_TYPE,
    TK_MAIN,
    TK_GLOBAL,
    TK_PARAMETER,
    TK_LIST,
    TK_INPUT,
    TK_OUTPUT,
    TK_INT,
    TK_REAL,
    TK_IF,
    TK_THEN,
    TK_ENDWHILE,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN,
    TK_CALL,
	TK_UNKNOWN
} TOKEN_TYPE; 

typedef struct Entry {
    char* key;
    TOKEN_TYPE value;
    struct Entry* next;
} Entry;

typedef struct
{
	char *buf;
	int init;
	int fwd;
	int isLastBuffer;
	FILE *fp;
} TwinBuffer;

typedef struct TOKEN // Structure of the token
{   
	TOKEN_TYPE token_type; 
	char lexeme[MAX_LEXEME_SIZE]; // DOUBT
	int line_number;
	//TOKEN_VAL token_value;
	bool isEOF;
} TOKEN;



#endif
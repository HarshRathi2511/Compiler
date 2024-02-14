#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "colorCodes.h"

#define BUF_SIZE 512
#define MAX_LEXEME_SIZE 30

typedef struct
{
	char *buf;
	int init;
	int fwd;
	int lastBufLoad;
	FILE *fp;
} TwinBuffer;

void initTwinBuffer(TwinBuffer *tb)
{
	tb->buf = malloc(sizeof(char) * (BUF_SIZE * 2));
	memset(tb->buf, 0, BUF_SIZE * 2);
	tb->fwd = 0;
	tb->lastBufLoad = 0;
}

void bufferLoader(TwinBuffer *tb, bool loadFirst)
{
	char *bufferPosition = loadFirst ? tb->buf : tb->buf + BUF_SIZE;
	size_t bytesRead = fread(bufferPosition, sizeof(char), BUF_SIZE, tb->fp);

	// If fewer characters were read than the buffer size, mark the end of the buffer with EOF
	if (bytesRead < BUF_SIZE)
	{
		bufferPosition[bytesRead] = EOF;
	}
}

void checkAndLoadBuffer(TwinBuffer *tb)
{
	// Check if the current position is at the end of the first buffer and the last buffer hasn't been loaded yet
	if ((tb->fwd % (2 * BUF_SIZE)) == (BUF_SIZE - 1) && !tb->lastBufLoad)
	{
		bufferLoader(tb, false);
		tb->lastBufLoad = 1;
	}
	// Check if the current position is at the end of the second buffer and the last buffer has been loaded
	else if ((tb->fwd % (2 * BUF_SIZE)) == (2 * BUF_SIZE - 1) && tb->lastBufLoad)
	{
		bufferLoader(tb, true);
		tb->lastBufLoad = 0;
	}
}

void printBufferContents(TwinBuffer *tb)
{
	int i;
	for (i = 0; i < 2 * BUF_SIZE; i++)
	{
		if (tb->buf[i] != EOF)
		{
			printf("%c", tb->buf[i]);
		}
		else
		{
			break; // Stop printing once EOF is encountered
		}
	}
}

int setupLexer(TwinBuffer *tb, FILE *fp)
{
	tb->fp = fp;
	initTwinBuffer(tb);
	bufferLoader(tb, true);
	return 0;
}

// token struct
typedef union TOKEN_VAL // Value field of the Token is any of these
{

	int number;
	double realnumber;
} TOKEN_VAL;

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
} TOKEN_TYPE; 

typedef struct TOKEN // Structure of the token
{   
	TOKEN_TYPE token_type; 
	char lexeme[MAX_LEXEME_SIZE]; // DOUBT
	unsigned int line_number;
	TOKEN_VAL token_value;
} TOKEN;

void printToken(const TOKEN *token) {
    printf(GREEN "Token Type: %d\n", token->token_type);
    printf("Lexeme: %s\n", token->lexeme);
    printf("Line Number: %u\n", token->line_number);
	printf("\n"); 
}

static int state = 1; // 1 is the init state
static int line_number = 1; 
TOKEN *getNextToken(TwinBuffer *tb)
{

	TOKEN *token = malloc(sizeof(TOKEN));
	char curr;
	int i = 0; // this is the lexeme offset
	while (1)
	{
		curr = tb->buf[tb->fwd % (2 * BUF_SIZE)];

		switch (state) {
			case 1: {
				if(curr == ' ' || curr == '\t' || curr == '\r'){
					state = 1; 
				}else if(curr == '\n'){
					state = 1; 
					line_number++;
				}
				else if (curr == '=') {
					state = 2;
					token->lexeme[i++] = curr;
				}else if(curr == '!'){
					state = 4; 
					token->lexeme[i++] = curr; 
				}else if(curr == '~'){
					state = 6; 
					token->lexeme[i++] = curr; 
				}else if(curr == '&'){
					state = 7;
					token->lexeme[i++] = curr; 
				}else if(curr == '@'){
					state = 10; 
					token->lexeme[i++] = curr;
				}else if(curr == '+'){
					state = 13; 
					token->lexeme[i++] = curr; 
				}else if(curr == '-'){
					state = 14; 
					token->lexeme[i++] = curr; 
				}else if(curr == '*'){
					state = 15; 
					token->lexeme[i++] = curr; 
				}else if(curr == '/'){
					state = 16; 
					token->lexeme[i++] = curr; 
				}else if(curr == '.'){
					state = 17; 
					token->lexeme[i++] = curr; 
				}else if(curr == ','){
					state = 18; 
					token->lexeme[i++] = curr; 
				}else if(curr == ':'){
					state = 19; 
					token->lexeme[i++] = curr; 
				}else if(curr == ';'){
					state = 20; 
					token->lexeme[i++] = curr; 
				}else if(curr == ')'){
					state = 21; 
					token->lexeme[i++] = curr; 
				}else if(curr == '('){
					state = 22; 
					token->lexeme[i++] = curr; 
				}else if(curr == ']'){
					state = 23; 
					token->lexeme[i++] = curr; 
				}else if(curr == '['){
					state = 24; 
					token->lexeme[i++] = curr; 
				}else if(curr == '%'){
					state = 25; 
					//no need for a lexeme here as ignore comments
				}else if(curr == '>'){
					state = 27; 
					token->lexeme[i++] = curr; 
				}else if(curr == '<'){
					state = 30; 
					token->lexeme[i++] = curr; 
				}
				//write other cases here
				break;
			}
			case 2: {
				if(curr == '='){
					state = 3; 
					token->lexeme[i++] = curr; 
				}else{
					//break into error 

				}	
				break;			
			}
			case 3: {
				//accept state for == 
				state = 1; 
				token->lexeme[i] = '\0'; 
				token->token_type = TK_EQ; 
				token->line_number = line_number; 

				return token;
			}
			case 4: {
				if(curr == '='){
					state = 5; 
					token->lexeme[i++] = curr; 
				}else{
					//report lexical error
				}
				break; 
			}
			case 5: {
				//accept state for != 
				state = 1; 
				token->lexeme[i] = '\0'; 
				token->token_type = TK_NE; 
			    token->line_number = line_number; 

				return token;
			}
			case 6: {
				//accept state for ~
				state = 1; 
				token->lexeme[i] = '\0'; 
				token->token_type = TK_NOT; 
				token->line_number = line_number;

				return token;
			}
			case 7: {
				if(curr == '&'){
					state = 8; 
					token->lexeme[i++] = curr; 
				}else{
					//report lexical error 
				}
				break;
			}
			case 8: {
				if(curr == '&'){
					state = 9; 
					token->lexeme[i++] = curr; 
				}else{
					//report lexical error 
				}
				break;
			}
			case 9: {
				//accept state for &&&
			    state = 1; 
				token->lexeme[i] = '\0'; 
				token->token_type = TK_NOT; 
				token->line_number = line_number;

				return token;
			}
			case 10: {
				if(curr == '@'){
					state = 11; 
					token->lexeme[i++] = curr; 
				}else{
					//report lexical error 
				}
				break;
			}
			case 11: {
				if(curr == '@'){
					state = 12; 
					token->lexeme[i++] = curr; 
				}else{
					//report lexical error 
				}
				break;
			}
			case 12: {
				//accept state for OR
			    state = 1; 
				token->lexeme[i] = '\0'; 
				token->token_type = TK_OR; 
				token->line_number = line_number;

				return token;				
			}
			case 13: {
				//accept state for +
			    state = 1; 
				token->lexeme[i] = '\0'; 
				token->token_type = TK_PLUS; 
				token->line_number = line_number;

				return token;					
			}
			case 14: {
				//accept state for -
			    state = 1; 
				token->lexeme[i] = '\0'; 
				token->token_type = TK_MINUS; 
				token->line_number = line_number;

				return token;		
			}
			case 15: {
				//accept state for *
			    state = 1; 
				token->lexeme[i] = '\0'; 
				token->token_type = TK_MUL; 
				token->line_number = line_number;

				return token;	
			}
			case 16: {
				//accept state for /
			    state = 1; 
				token->lexeme[i] = '\0'; 
				token->token_type = TK_DIV; 
				token->line_number = line_number;

				return token;	
			}
			case 17: {
				//accept state for .
			    state = 1; 
				token->lexeme[i] = '\0'; 
				token->token_type = TK_DOT; 
				token->line_number = line_number;

				return token;	
			}
			case 18: {
				//accept state for ,
			    state = 1; 
				token->lexeme[i] = '\0'; 
				token->token_type = TK_COMMA; 
				token->line_number = line_number;

				return token;	
			}
			case 19: {
				//accept state for :
			    state = 1; 
				token->lexeme[i] = '\0'; 
				token->token_type = TK_COLON; 
				token->line_number = line_number;

				return token;	
			}			
			case 20: {
				//accept state for .
			    state = 1; 
				token->lexeme[i] = '\0'; 
				token->token_type = TK_SEM; 
				token->line_number = line_number;

				return token;	
			}
			case 21: {
				//accept state for .
			    state = 1; 
				token->lexeme[i] = '\0'; 
				token->token_type = TK_CL; 
				token->line_number = line_number;

				return token;	
			}
			case 22: {
			    state = 1; 
				token->lexeme[i] = '\0'; 
				token->token_type = TK_OP; 
				token->line_number = line_number;

				return token;	
			}
			case 23: {
			    state = 1; 
				token->lexeme[i] = '\0'; 
				token->token_type = TK_SQR; 
				token->line_number = line_number;

				return token;	
			}	
			case 24: {
			    state = 1; 
				token->lexeme[i] = '\0'; 
				token->token_type = TK_SQL; 
				token->line_number = line_number;

				return token;	
			}	
			case 25: {
				if(curr != '\n'){
					state = 25; //keep coming to this state
				}else{
					//when we found a new line 
					//retract one step 
					tb->fwd--; 
					state = 1; //re read the \n again 
				}
				break;
			}
			case 27: {
				if(curr == '='){
					state = 29; 
					token->lexeme[i++] = curr; 
				}else{
					//! take care of the case where the other character read is \n
					//i.e 
					// > 
					// a
					// if(curr == '\n'){
					// 	line_number++;
					// }
					state = 28; 
					//dont store the other character in the lexeme 
				}
				break;
			}
			case 28: {
				//retract one step,mark the state 1 and then return the TK_GT step
				tb->fwd--;
				state = 1; 

				token->lexeme[i] = '\0'; 
				token->token_type = TK_GT;
				token->line_number = line_number;

				return token;
			}	
			case 29: {
				//accept state for >=
				state = 1; 

				token->lexeme[i] = '\0'; 
				token->token_type = TK_GE;
				token->line_number = line_number;

				return token;
			}	
			case 30: {
				if(curr == '='){
					state = 32; 
					token->lexeme[i++] = curr; 
				}else if(curr == '-'){
					state = 33; 
					token->lexeme[i++] = curr; 
				}else{
					//other case 
					state = 31; 
				}
				break;
			}
			case 31: {
				//retract one step
				tb->fwd--; 
				state = 1; 

				//also return the token 
				token->lexeme[i] = '\0'; 
				token->token_type = TK_LT;
				token->line_number = line_number;	

				return token; 			
			}
			case 32: {
				//accept state for TK_LE
				state  = 1; 
				token->lexeme[i] = '\0'; 
				token->token_type = TK_LE;
				token->line_number = line_number;
			    
				return token;
			}
			case 33: {
				if(curr == '-'){
					token->lexeme[i++] = curr; 
					state = 34; 
				}else{
					//go to state 36 and then double retract from there
					state = 36; 
				}
				break;
			}
			case 36: {
				//double retract here 
				tb->fwd--; 
				tb->fwd--; 

				state = 1;
                
				//also remove one char from the lexeme
				token->lexeme[--i] = '\0'; 
				token->token_type = TK_LT;
				token->line_number = line_number;

				return token;
			}								
			case 404: {
				//Lexical error state

			}
		}

		// printf("%c", curr);

		// when curr is EOF then break
		if (curr == EOF)
		{
			return NULL; 
		}
        
		//at the end it is by default forwading
		tb->fwd++;
		checkAndLoadBuffer(tb);
	}

	return token;
}

int main()
{
	TwinBuffer twinBuffer;
	FILE *inputFile = fopen("test.txt", "r");
	if (inputFile == NULL)
	{
		fprintf(stderr, "Error opening input file.\n");
		return 1;
	}

	if (setupLexer(&twinBuffer, inputFile) != 0)
	{
		fprintf(stderr, "Error setting up lexer.\n");
		fclose(inputFile);
		return 1;
	}
    

	// TOKEN* t1 = getNextToken(&twinBuffer);
	// printToken(t1); 
	// TOKEN* t2 = getNextToken(&twinBuffer); 
	// printToken(t2); 
	while(true){
		TOKEN* token = getNextToken(&twinBuffer);
        if(token == NULL){
			break;
		}
		printToken(token); 
	}

	fclose(inputFile);

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "colorCodes.h"

#define BUF_SIZE 512
#define MAX_LEXEME_SIZE 512

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
	TK_FUNID,
	TK_FIELDID,
	TK_RUID,
	TK_ID,
	TK_NUM,
	TK_RNUM,
	TK_ERROR
} TOKEN_TYPE; 

typedef struct TOKEN // Structure of the token
{   
	TOKEN_TYPE token_type; 
	char lexeme[MAX_LEXEME_SIZE]; // DOUBT
	unsigned int line_number;
	TOKEN_VAL token_value;
	bool isEOF;
} TOKEN;

const char* token_type_to_string(int token_type) {
    switch (token_type) {
        case TK_EQ: return "TK_EQ";
        case TK_NE: return "TK_NE";
        case TK_NOT: return "TK_NOT";
        case TK_AND: return "TK_AND";
        case TK_OR: return "TK_OR";
        case TK_PLUS: return "TK_PLUS";
        case TK_MINUS: return "TK_MINUS";
        case TK_MUL: return "TK_MUL";
        case TK_DIV: return "TK_DIV";
        case TK_DOT: return "TK_DOT";
        case TK_COMMA: return "TK_COMMA";
        case TK_COLON: return "TK_COLON";
        case TK_SEM: return "TK_SEM";
        case TK_CL: return "TK_CL";
        case TK_OP: return "TK_OP";
        case TK_SQR: return "TK_SQR";
        case TK_SQL: return "TK_SQL";
        case TK_COMMENT: return "TK_COMMENT";
        case TK_GT: return "TK_GT";
        case TK_GE: return "TK_GE";
        case TK_LT: return "TK_LT";
        case TK_LE: return "TK_LE";
        case TK_ASSIGNOP: return "TK_ASSIGNOP";
        case TK_FUNID: return "TK_FUNID";
        case TK_FIELDID: return "TK_FIELDID";
        case TK_RUID: return "TK_RUID";
        case TK_ID: return "TK_ID";
        case TK_NUM: return "TK_NUM";
        case TK_RNUM: return "TK_RNUM";
        default: return "UNKNOWN";
    }
}
void printToken(const TOKEN *token) {
	if(token->token_type == TK_ERROR){
		printf(RED "Line Number: %u Error: %s \n", token->line_number, token->lexeme);
	}else{
	   printf(GREEN"Line Number: %u    Lexeme %s    Token %s \n", token->line_number, token->lexeme, token_type_to_string(token->token_type));
	}
    printf("\n"); 
}

static int state = 1; // 1 is the init state
static int line_number = 1; 
TOKEN *getNextToken(TwinBuffer *tb)
{

	TOKEN *token = malloc(sizeof(TOKEN));
	token->isEOF = false;
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
				}else if(curr == EOF){
					token->isEOF = true;
					return token;
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
					token->lexeme[i++] = curr; 
					//no need for a lexeme here as ignore comments
				}else if(curr == '>'){
					state = 27; 
					token->lexeme[i++] = curr; 
				}else if(curr == '<'){
					state = 30; 
					token->lexeme[i++] = curr; 
				}else if (curr == '_'){
					state = 37;
					token->lexeme[i++] = curr;		
				}else if(curr == '#'){
					state = 41; 
					token->lexeme[i++] = curr; 
				}
				//! need to fix 44 and 46
				else if(curr == 'a' || curr >= 'e' && curr <= 'z'){
					state = 44; 
					//token->lexeme[i++]='\0';
					token->lexeme[i++] = curr; 
				}else if(curr >= 'b' && curr <= 'd'){
					state = 46; 
					token->lexeme[i++] = curr;
				}else if((curr - '0') >= 0 && (curr - '0') <= 9){
					state = 50; 
					token->lexeme[i++] = curr; 
				}else{
					state = 404; 
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
					state = 1; 
					token->line_number = line_number; 
					token->token_type = TK_ERROR; 
					strcpy(token->lexeme, "Unknown symbol <=>");
					return token;
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
					//break into error 
					state = 1; 
					token->line_number = line_number; 
					token->token_type = TK_ERROR; 
					strcpy(token->lexeme, "Unknown symbol <!>");
					return token;
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
					//break into error 
					state = 1; 
					token->line_number = line_number; 
					token->token_type = TK_ERROR; 
					strcpy(token->lexeme, "Unknown symbol <&>");
					return token;
				}
				break;
			}
			case 8: {
				if(curr == '&'){
					state = 9; 
					token->lexeme[i++] = curr; 
				}else{
					//break into error 
					state = 1; 
					token->line_number = line_number; 
					token->token_type = TK_ERROR; 
					strcpy(token->lexeme, "Unknown symbol <&&>");
					return token;
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
					//break into error 
					state = 1; 
					token->line_number = line_number; 
					token->token_type = TK_ERROR; 
					strcpy(token->lexeme, "Unknown symbol <@>");
					return token; 
				}
				break;
			}
			case 11: {
				if(curr == '@'){
					state = 12; 
					token->lexeme[i++] = curr; 
				}else{
					//break into error 
					state = 1; 
					token->line_number = line_number; 
					token->token_type = TK_ERROR; 
					strcpy(token->lexeme, "Unknown symbol <@@>");
					return token; 
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
					// tb->fwd--; //to re read the \n again as single point of updation of line number
					state = 1; //re read the \n again 

				    strcpy(token->lexeme, "%"); 
					token->token_type = TK_COMMENT;
					token->line_number = line_number;
					return token;
				}
				break;
			}
			case 27: {
				if(curr == '='){
					state = 29; 
					token->lexeme[i++] = curr; 
				}else{
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
				//printf("in case 30");
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
			case 34: {
				if(curr == '-'){
					token->lexeme[i++] = curr; 
					state = 35; 
				}else{
					//report a lexical error
					//break into error 
					state = 1; 
					token->line_number = line_number; 
					token->token_type = TK_ERROR; 
					strcpy(token->lexeme, "Unknown symbol < <-- >");
					return token; 
				}
				break;
			}
			case 35: {
				//accept state for assignop 
				state = 1;
                
				//also remove one char from the lexeme
				token->lexeme[i] = '\0'; 
				token->token_type = TK_ASSIGNOP;
				token->line_number = line_number;

				return token;
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
			case 37: {
				if(curr >= 'a' && curr <= 'z'  || curr >= 'A' && curr <= 'Z'){
					state = 38; 
					token->lexeme[i++] = curr; 
				}else{	
					//! break into error 
					//! this might take some more space
					state = 1; 
					token->lexeme[i] = '\0';
					token->line_number = line_number; 
					token->token_type = TK_ERROR; 
					char* errorMessage = malloc(sizeof(char) * 100);
					sprintf(errorMessage, "Unknown pattern <%s>", token->lexeme); 
					strcpy(token->lexeme,errorMessage);
					return token; 			
				}
				break;
			}	
			case 38: {
				if(curr >= 'a' && curr <= 'z'  || curr >= 'A' && curr <= 'Z'){
					state = 38; 
					token->lexeme[i++] = curr; 
				}else if((curr - '0') >= 0 && (curr - '0') <= 9){
					state = 39; 
					token->lexeme[i++] = curr; 								
				}else{
					//go here and then retract and make the token 
					state = 40;
				}
				break;				
			}	
			case 39: {
	            if((curr - '0') >= 0 && (curr - '0') <= 9){
					state = 39; 
					token->lexeme[i++] = curr; 	
				}else{
					//go to state 40 and make the function token 
					state = 40; 
				}	
				break;			
			}
			case 40: {
				//!how to use the lookup functionality
				//retract one step
				tb->fwd--; 
				state = 1; 
				//also return the token 
				//? perform a lookup here (not sure)
				token->lexeme[i] = '\0'; 
				token->line_number = line_number;
				if(strlen(token->lexeme) > 30){
					token->token_type = TK_ERROR;
					char* errorMessage = "Function Identifier is longer than the prescribed length of 30 characters";
					strcpy(token->lexeme,errorMessage);
					return token;
				}
				token->token_type = TK_FUNID;
				return token; 
			}	
			case 41: {
				if(curr >= 'a' && curr <= 'z'){
					state = 42; 
					token->lexeme[i++] = curr; 
				}else{
					//break into error 
					state = 1; 
					token->lexeme[i] = '\0';
					token->line_number = line_number; 
					token->token_type = TK_ERROR; 
					char* errorMessage = malloc(sizeof(char) * 100);
					sprintf(errorMessage, "Unknown pattern <%s>", token->lexeme); 
					strcpy(token->lexeme,errorMessage);
					return token; 	
				}
				break;
			}
			case 42: {
				if(curr >= 'a' && curr <= 'z'){
					state = 42; 
					token->lexeme[i++] = curr; 
				}else{
					//goto 43 and tetract
					state = 43;	
				}
				break;				
			}
			case 43: {
				tb->fwd--;
				state = 1; 

				//return token record
				token->lexeme[i] = '\0'; 
				token->token_type = TK_RUID;
				token->line_number = line_number;	

				return token;			
			}	
			case 44: {
				if(curr >= 'a' && curr <='z'){
					state = 44; 
					token->lexeme[i++] = curr;
				}else{
					//goto 45 and retract
					state = 45;
				}
				break;
			}	
			case 45: {
				tb->fwd--; 
				state = 1; 
				//! implement lookup functionality

				token->lexeme[i] = '\0'; 
				token->token_type = TK_FIELDID;
				token->line_number = line_number;	

				return token;	


			}
			case 46:{
				if(curr >= 'a' && curr <='z'){
					state = 44; 
					token->lexeme[i++] = curr;
				}else if ((curr - '0') >= 2 && (curr - '0') <= 7){
					
					state = 47;
					token->lexeme[i++] = curr;
				}else{
					state= 63; // go to 63 and retract
				}
				break;
			}
			case 63:{
				tb->fwd--; 
				state = 1; 
				//! implement lookup functionality

				token->lexeme[i] = '\0'; 
				token->token_type = TK_FIELDID;
				token->line_number = line_number;	

				return token;	
			}
			case 47: {
				if(curr >= 'b' && curr <='d'){
					state = 47; 
					token->lexeme[i++] = curr;
				}else if ((curr - '0') >= 2 && (curr - '0') <= 7){
					
					state = 48;
					token->lexeme[i++] = curr;
				}else{
					state= 49; // go to 49 and retract
				}

				break;

			}
			case 48: {
				if ((curr - '0') >= 2 && (curr - '0') <= 7){
					
					state = 48;
					token->lexeme[i++] = curr;
				}else{
					state= 49; // go to 49 and retract
				}

				break;

			}
			case 49 : {
				tb->fwd--; 
				state = 1; 
				//! implement lookup functionality

				token->lexeme[i] = '\0'; 
				if(strlen(token->lexeme) > 20){
					token->token_type = TK_ERROR;
					char* errorMessage = "Variable Identifier is longer than the prescribed length of 20 characters";
					strcpy(token->lexeme,errorMessage);
					return token;
				}
				token->token_type = TK_ID;
				token->line_number = line_number;	

				return token;
			}

			case 50: {
				if((curr - '0') >= 0 && (curr - '0') <= 9){
					state = 50; 
					token->lexeme[i++] = curr; 
				}else if(curr == '.'){
					state = 52; 
					token->lexeme[i++] = curr; 
				}else{
					state = 51; //retract 
				}
				break;
			}	
			case 51: {
				tb->fwd--; 
				state = 1; 
				//return token record
				token->lexeme[i] = '\0'; 
				token->token_type = TK_NUM;
				token->line_number = line_number;

				//check if we need to fill the union tyoe 
				return token;
			}	
			case 52: {
	            if((curr - '0') >= 0 && (curr - '0') <= 9){
					state = 54; 
					token->lexeme[i++] = curr; 
				}else{
					state = 53; //retract 
				}
				break;				
			}
			case 53: {
				//double retract here 
				tb->fwd--; 
				tb->fwd--; 

				state = 1;
                
				//also remove one char from the lexeme
				token->lexeme[--i] = '\0'; 
				token->token_type = TK_NUM;
				token->line_number = line_number;

				return token;				
			}
			case 54: {
		        if((curr - '0') >= 0 && (curr - '0') <= 9){
					state = 55; 
					token->lexeme[i++] = curr; 
				}else{
					state = 1; 
					token->lexeme[i] = '\0';
					token->line_number = line_number; 
					token->token_type = TK_ERROR; 
					char* errorMessage = malloc(sizeof(char) * 100);
					sprintf(errorMessage, "Unknown pattern <%s>", token->lexeme); 
					strcpy(token->lexeme,errorMessage);
					return token; 	
				}
				break;			
			}
			case 55:{
				if(curr == 'E'){
					state = 57; 
					token->lexeme[i++] = curr; 
				}else{
					state = 56; //retract
				}
				break;
			}
			case 56: {
				tb->fwd--; 
				state = 1; 

				token->lexeme[i] = '\0'; 
				token->token_type = TK_RNUM;
				token->line_number = line_number;

				return token;
			}
			case 57: {
				if(curr == '+' || curr == '-'){
					state = 58; 
					token->lexeme[i++] = curr; 
				}else if((curr - '0') >=0 && (curr - '0') <= 9){
					state = 59; 
					token->lexeme[i++] = curr; 
				}else{
					//report a lexical error here
					state = 1; 
					token->lexeme[i] = '\0';
					token->line_number = line_number; 
					token->token_type = TK_ERROR; 
					char* errorMessage = malloc(sizeof(char) * 100);
					sprintf(errorMessage, "Unknown pattern <%s>", token->lexeme); 
					strcpy(token->lexeme,errorMessage);
					return token; 				
				}
				break;
			}
			case 58: {
		        if((curr - '0') >= 0 && (curr - '0') <= 9){
					state = 59; 
					token->lexeme[i++] = curr; 
				}else{
					//report a lexical error here
					state = 1; 
					token->lexeme[i] = '\0';
					token->line_number = line_number; 
					token->token_type = TK_ERROR; 
					char* errorMessage = malloc(sizeof(char) * 100);
					sprintf(errorMessage, "Unknown pattern <%s>", token->lexeme); 
					strcpy(token->lexeme,errorMessage);
					return token; 
				}
				break;				
			}
			case 59: {
		        if((curr - '0') >= 0 && (curr - '0') <= 9){
					state = 60; 
					token->lexeme[i++] = curr; 
				}else{
					//report a lexical error here
					state = 1; 
					token->lexeme[i] = '\0';
					token->line_number = line_number; 
					token->token_type = TK_ERROR; 
					char* errorMessage = malloc(sizeof(char) * 100);
					sprintf(errorMessage, "Unknown pattern <%s>", token->lexeme); 
					strcpy(token->lexeme,errorMessage);
					return token; 
				}
				break;				
			}
			case 60: {
				//! if we get a number here do we have to report a lex error ?
				if(((curr - '0') >= 0 && (curr - '0') <= 9) == 0){
					state = 61; 
				}else{
					//? error this 
					state = 61;
				}
				break;
			}
			case 61: {
				tb->fwd--; 
				state = 1; 

				token->lexeme[i] = '\0'; 
				token->token_type = TK_RNUM;
				token->line_number = line_number;

				return token;
			}

			case 404:{
				//report a lexical error here
				state = 1; 
				token->lexeme[i] = '\0';
				token->line_number = line_number; 
				token->token_type = TK_ERROR; 
				char* errorMessage = malloc(sizeof(char) * 100);
				sprintf(errorMessage, "Unknown pattern <%s>", token->lexeme); 
				strcpy(token->lexeme,errorMessage);
				return token; 
			}

			default: {
				// //when we read anything other than the above mentioned characters
				// 				//report a lexical error here
				// state = 1; 
				// token->lexeme[i] = '\0';
				// token->line_number = line_number; 
				// token->token_type = TK_ERROR; 
				// char* errorMessage = malloc(sizeof(char) * 100);
				// sprintf(errorMessage, "Unknown pattern <%s>", token->lexeme); 
				// strcpy(token->lexeme,errorMessage);
				// return token; 
				// break; 
			}
		}
		//at the end it is by default forwading
		tb->fwd++;
		
		// printf("the fwd is pointing to %c\n", curr); 
		checkAndLoadBuffer(tb);
	}
    
	//if we are returning from here then token has an error
	// strcpy(token->lexeme, "error"); 
	return NULL;
}

int main()
{
	TwinBuffer twinBuffer;
	FILE *inputFile = fopen("t1.txt", "r");
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
        if(token->isEOF){
			break;
		}
		printToken(token); 
	}

	fclose(inputFile);

	return 0;
}

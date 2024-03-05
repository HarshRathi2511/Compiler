#include "colorCodes.h"
#include "lexerDef.h"
#include "firstandfollow.h"
// REMOVING COMMENTS
char *Find_Comment_Symbol(char comment, const char *readBuffer)
{
	int counter = 0;
	while (*readBuffer != '\0')
	{
		int counter_two = 1;
		if (counter == 0)
		{
			counter--;
			counter_two++;
		}
		if (*readBuffer == comment)
		{
			return (char *)readBuffer;
		}
		counter++;
		readBuffer++;
		counter_two++;
	}
	counter++;
	return NULL;
}

void Remove_Comments(char *inputFile, char *outputFile)
{
	char readBuffer[len];
	FILE *in = fopen(inputFile, "r");
	FILE *out = fopen(outputFile, "w");
	while (fgets(readBuffer, sizeof(readBuffer), in) != NULL)
	{
		// find a % (comment symbol) by traversing
		char *comment_sign;
		int no = 1;
		char comment[2000];
		char *com_ptr = Find_Comment_Symbol('%', readBuffer);
		// comment_sign = "%";
		// if (comment_sign != "%")
		// {
		// 	// comment="\n";
		// 	break;
		// }
		if (com_ptr != NULL)
		{
			// if % exists, replace everything after that symbol with \n
			no++;
			*com_ptr = '\n';
			*(com_ptr + 1) = '\0';
		}
		no--;
		fprintf(out, "%s", readBuffer);
		printf("%s", readBuffer);
	}

	fclose(in);
	fclose(out);
}

// MAKING OF SYMBOL TABLE AND INSERTING KEYWORDS INTO IT
Entry *hashTable[HASH_TABLE_SIZE];

// Hash function
unsigned int hash(char *key)
{
	unsigned int hashValue = 0;
	for (int i = 0; key[i]; i++)
	{
		hashValue = 31 * hashValue + key[i];
	}
	return hashValue % HASH_TABLE_SIZE;
}

// Insert into hash table
void insert(char *key, TOKEN_TYPE value)
{
	unsigned int index = hash(key);

	Entry *newEntry = (Entry *)malloc(sizeof(Entry));
	newEntry->key = strdup(key);
	newEntry->value = value;
	newEntry->next = NULL;

	// Insert at beginning(if no collision), else add to list
	if (hashTable[index] == NULL)
	{
		hashTable[index] = newEntry;
	}
	else
	{
		// Add to the beginning of the list
		newEntry->next = hashTable[index];
		hashTable[index] = newEntry;
	}
}

// Search for an entry in the hash table
Entry *search(char *key)
{
	unsigned int index = hash(key);
	Entry *current = hashTable[index];
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			return current;
		}
		current = current->next;
	}
	return NULL;
}

void AddtoHashTable()
{
	insert("else", TK_ELSE);
	insert("endrecord", TK_ENDRECORD);
	insert("record", TK_RECORD);
	insert("with", TK_WITH);
	insert("parameters", TK_PARAMETERS);
	insert("end", TK_END);
	insert("while", TK_WHILE);
	insert("union", TK_UNION);
	insert("endunion", TK_ENDUNION);
	insert("definetype", TK_DEFINETYPE);
	insert("as", TK_AS);
	insert("type", TK_TYPE);
	insert("_main", TK_MAIN);
	insert("global", TK_GLOBAL);
	insert("int", TK_INT);
	insert("real", TK_REAL);
	insert("if", TK_IF);
	insert("then", TK_THEN);
	insert("endwhile", TK_ENDWHILE);
	insert("endif", TK_ENDIF);
	insert("read", TK_READ);
	insert("write", TK_WRITE);
	insert("return", TK_RETURN);
	insert("call", TK_CALL);
	insert("parameter", TK_PARAMETER);
	insert("list", TK_LIST);
	insert("input", TK_INPUT);
	insert("output", TK_OUTPUT);
}

// endhash

int getCurrBuffPtr(TwinBuffer *tb)
{
	return tb->fwd % (2 * BUF_SIZE);
}

int getMaxBufSize(){
	return 512;
}

void initTwinBuffer(TwinBuffer *tb)
{   
	int buff_size = getMaxBufSize();
	tb->buf = malloc(sizeof(char) * (buff_size * 2));
	memset(tb->buf, 0, buff_size * 2);
	tb->fwd = 0;
	tb->isLastBuffer = 0;
}

void bufferLoader(TwinBuffer *tb, bool flag)
{   
	int buff_size = getMaxBufSize();
	char *bufferPosition = flag ? tb->buf : tb->buf + buff_size;
	size_t bytesRead = fread(bufferPosition, sizeof(char), buff_size, tb->fp);

	// If fewer characters were read than the buffer size, mark the end of the buffer with EOF
	if (bytesRead < buff_size)
	{
		bufferPosition[bytesRead] = EOF;
	}
}

TOKEN_TYPE string_to_token_type(const char *str)
{
	if (strcmp(str, "TK_EQ") == 0)
		return TK_EQ;
	if (strcmp(str, "TK_NE") == 0)
		return TK_NE;
	if (strcmp(str, "TK_NOT") == 0)
		return TK_NOT;
	if (strcmp(str, "TK_AND") == 0)
		return TK_AND;
	if (strcmp(str, "TK_OR") == 0)
		return TK_OR;
	if (strcmp(str, "TK_PLUS") == 0)
		return TK_PLUS;
	if (strcmp(str, "TK_MINUS") == 0)
		return TK_MINUS;
	if (strcmp(str, "TK_MUL") == 0)
		return TK_MUL;
	if (strcmp(str, "TK_DIV") == 0)
		return TK_DIV;
	if (strcmp(str, "TK_DOT") == 0)
		return TK_DOT;
	if (strcmp(str, "TK_COMMA") == 0)
		return TK_COMMA;
	if (strcmp(str, "TK_COLON") == 0)
		return TK_COLON;
	if (strcmp(str, "TK_SEM") == 0)
		return TK_SEM;
	if (strcmp(str, "TK_CL") == 0)
		return TK_CL;
	if (strcmp(str, "TK_OP") == 0)
		return TK_OP;
	if (strcmp(str, "TK_SQR") == 0)
		return TK_SQR;
	if (strcmp(str, "TK_SQL") == 0)
		return TK_SQL;
	if (strcmp(str, "TK_COMMENT") == 0)
		return TK_COMMENT;
	if (strcmp(str, "TK_GT") == 0)
		return TK_GT;
	if (strcmp(str, "TK_GE") == 0)
		return TK_GE;
	if (strcmp(str, "TK_LT") == 0)
		return TK_LT;
	if (strcmp(str, "TK_LE") == 0)
		return TK_LE;
	if (strcmp(str, "TK_ASSIGNOP") == 0)
		return TK_ASSIGNOP;
	if (strcmp(str, "TK_FUNID") == 0)
		return TK_FUNID;
	if (strcmp(str, "TK_FIELDID") == 0)
		return TK_FIELDID;
	if (strcmp(str, "TK_RUID") == 0)
		return TK_RUID;
	if (strcmp(str, "TK_ID") == 0)
		return TK_ID;
	if (strcmp(str, "TK_NUM") == 0)
		return TK_NUM;
	if (strcmp(str, "TK_RNUM") == 0)
		return TK_RNUM;
	if (strcmp(str, "TK_ELSE") == 0)
		return TK_ELSE;
	if (strcmp(str, "TK_ENDRECORD") == 0)
		return TK_ENDRECORD;
	if (strcmp(str, "TK_RECORD") == 0)
		return TK_RECORD;
	if (strcmp(str, "TK_WITH") == 0)
		return TK_WITH;
	if (strcmp(str, "TK_PARAMETERS") == 0)
		return TK_PARAMETERS;
	if (strcmp(str, "TK_END") == 0)
		return TK_END;
	if (strcmp(str, "TK_WHILE") == 0)
		return TK_WHILE;
	if (strcmp(str, "TK_UNION") == 0)
		return TK_UNION;
	if (strcmp(str, "TK_ENDUNION") == 0)
		return TK_ENDUNION;
	if (strcmp(str, "TK_DEFINETYPE") == 0)
		return TK_DEFINETYPE;
	if (strcmp(str, "TK_AS") == 0)
		return TK_AS;
	if (strcmp(str, "TK_TYPE") == 0)
		return TK_TYPE;
	if (strcmp(str, "TK_MAIN") == 0)
		return TK_MAIN;
	if (strcmp(str, "TK_GLOBAL") == 0)
		return TK_GLOBAL;
	if (strcmp(str, "TK_PARAMETER") == 0)
		return TK_PARAMETER;
	if (strcmp(str, "TK_LIST") == 0)
		return TK_LIST;
	if (strcmp(str, "TK_INPUT") == 0)
		return TK_INPUT;
	if (strcmp(str, "TK_OUTPUT") == 0)
		return TK_OUTPUT;
	if (strcmp(str, "TK_INT") == 0)
		return TK_INT;
	if (strcmp(str, "TK_REAL") == 0)
		return TK_REAL;
	if (strcmp(str, "TK_IF") == 0)
		return TK_IF;
	if (strcmp(str, "TK_THEN") == 0)
		return TK_THEN;
	if (strcmp(str, "TK_ENDWHILE") == 0)
		return TK_ENDWHILE;
	if (strcmp(str, "TK_ENDIF") == 0)
		return TK_ENDIF;
	if (strcmp(str, "TK_READ") == 0)
		return TK_READ;
	if (strcmp(str, "TK_WRITE") == 0)
		return TK_WRITE;
	if (strcmp(str, "TK_RETURN") == 0)
		return TK_RETURN;
	if (strcmp(str, "TK_CALL") == 0)
		return TK_CALL;
	return TK_UNKNOWN;
}

void checkAndLoadBuffer(TwinBuffer *tb)
{   
	int buff_size = getMaxBufSize(); 
	// Check if the current position is at the end of the first buffer and the last buffer hasn't been loaded yet
	if (getCurrBuffPtr(tb) == (buff_size - 1) && !tb->isLastBuffer)
	{
		bufferLoader(tb, false);
		tb->isLastBuffer = 1;
	}
	// Check if the current position is at the end of the second buffer and the last buffer has been loaded
	else if (getCurrBuffPtr(tb) == (2 * buff_size - 1) && tb->isLastBuffer)
	{
		bufferLoader(tb, true);
		tb->isLastBuffer = 0;
	}
}

const char *token_type_to_string(TOKEN_TYPE token)
{
	switch (token)
	{
	case TK_EQ:
		return "TK_EQ";
	case TK_NE:
		return "TK_NE";
	case TK_NOT:
		return "TK_NOT";
	case TK_AND:
		return "TK_AND";
	case TK_OR:
		return "TK_OR";
	case TK_PLUS:
		return "TK_PLUS";
	case TK_MINUS:
		return "TK_MINUS";
	case TK_MUL:
		return "TK_MUL";
	case TK_DIV:
		return "TK_DIV";
	case TK_DOT:
		return "TK_DOT";
	case TK_COMMA:
		return "TK_COMMA";
	case TK_COLON:
		return "TK_COLON";
	case TK_SEM:
		return "TK_SEM";
	case TK_CL:
		return "TK_CL";
	case TK_OP:
		return "TK_OP";
	case TK_SQR:
		return "TK_SQR";
	case TK_SQL:
		return "TK_SQL";
	case TK_COMMENT:
		return "TK_COMMENT";
	case TK_GT:
		return "TK_GT";
	case TK_GE:
		return "TK_GE";
	case TK_LT:
		return "TK_LT";
	case TK_LE:
		return "TK_LE";
	case TK_ASSIGNOP:
		return "TK_ASSIGNOP";
	case TK_FUNID:
		return "TK_FUNID";
	case TK_FIELDID:
		return "TK_FIELDID";
	case TK_RUID:
		return "TK_RUID";
	case TK_ID:
		return "TK_ID";
	case TK_NUM:
		return "TK_NUM";
	case TK_RNUM:
		return "TK_RNUM";
	case TK_ERROR:
		return "TK_ERROR";
	case TK_ELSE:
		return "TK_ELSE";
	case TK_ENDRECORD:
		return "TK_ENDRECORD";
	case TK_RECORD:
		return "TK_RECORD";
	case TK_WITH:
		return "TK_WITH";
	case TK_PARAMETERS:
		return "TK_PARAMETERS";
	case TK_END:
		return "TK_END";
	case TK_WHILE:
		return "TK_WHILE";
	case TK_UNION:
		return "TK_UNION";
	case TK_ENDUNION:
		return "TK_ENDUNION";
	case TK_DEFINETYPE:
		return "TK_DEFINETYPE";
	case TK_AS:
		return "TK_AS";
	case TK_TYPE:
		return "TK_TYPE";
	case TK_MAIN:
		return "TK_MAIN";
	case TK_GLOBAL:
		return "TK_GLOBAL";
	case TK_PARAMETER:
		return "TK_PARAMETER";
	case TK_LIST:
		return "TK_LIST";
	case TK_INPUT:
		return "TK_INPUT";
	case TK_OUTPUT:
		return "TK_OUTPUT";
	case TK_INT:
		return "TK_INT";
	case TK_REAL:
		return "TK_REAL";
	case TK_IF:
		return "TK_IF";
	case TK_THEN:
		return "TK_THEN";
	case TK_ENDWHILE:
		return "TK_ENDWHILE";
	case TK_ENDIF:
		return "TK_ENDIF";
	case TK_READ:
		return "TK_READ";
	case TK_WRITE:
		return "TK_WRITE";
	case TK_RETURN:
		return "TK_RETURN";
	case TK_CALL:
		return "TK_CALL";
	default:
		return "UNKNOWN";
	}
}


void printBufferContents(TwinBuffer *tb)
{
	int i;
		int buff_size = getMaxBufSize(); 
	for (i = 0; i < 2 * buff_size; i++)
	{
		if (tb->buf[i] != EOF)
		{
			// printf("%c", tb->buf[i]);
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

void printToken(const TOKEN *token)
{
	if (token->token_type == TK_ERROR)
	{
		printf(RED "Line Number: %u Error: %s \n", token->line_number, token->lexeme);
	}
	else
	{
		printf(GREEN "Line Number: %u    Lexeme %s    Token %s \n", token->line_number, token->lexeme, token_type_to_string(token->token_type));
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

		switch (state)
		{
		case 1:
		{
			if (curr == ' ' || curr == '\t' || curr == '\r')
			{
				state = 1;
			}
			else if (curr == '\n')
			{
				state = 1;
				line_number++;
			}
			else if (curr == EOF)
			{
				token->isEOF = true;
				return token;
			}
			else if (curr == '=')
			{
				state = 2;
				token->lexeme[i++] = curr;
			}
			else if (curr == '!')
			{
				state = 4;
				token->lexeme[i++] = curr;
			}
			else if (curr == '~')
			{
				state = 6;
				token->lexeme[i++] = curr;
			}
			else if (curr == '&')
			{
				state = 7;
				token->lexeme[i++] = curr;
			}
			else if (curr == '@')
			{
				state = 10;
				token->lexeme[i++] = curr;
			}
			else if (curr == '+')
			{
				state = 13;
				token->lexeme[i++] = curr;
			}
			else if (curr == '-')
			{
				state = 14;
				token->lexeme[i++] = curr;
			}
			else if (curr == '*')
			{
				state = 15;
				token->lexeme[i++] = curr;
			}
			else if (curr == '/')
			{
				state = 16;
				token->lexeme[i++] = curr;
			}
			else if (curr == '.')
			{
				state = 17;
				token->lexeme[i++] = curr;
			}
			else if (curr == ',')
			{
				state = 18;
				token->lexeme[i++] = curr;
			}
			else if (curr == ':')
			{
				state = 19;
				token->lexeme[i++] = curr;
			}
			else if (curr == ';')
			{
				state = 20;
				token->lexeme[i++] = curr;
			}
			else if (curr == ')')
			{
				state = 21;
				token->lexeme[i++] = curr;
			}
			else if (curr == '(')
			{
				state = 22;
				token->lexeme[i++] = curr;
			}
			else if (curr == ']')
			{
				state = 23;
				token->lexeme[i++] = curr;
			}
			else if (curr == '[')
			{
				state = 24;
				token->lexeme[i++] = curr;
			}
			else if (curr == '%')
			{
				state = 25;
				token->lexeme[i++] = curr;
				// no need for a lexeme here as ignore comments
			}
			else if (curr == '>')
			{
				state = 27;
				token->lexeme[i++] = curr;
			}
			else if (curr == '<')
			{
				state = 30;
				token->lexeme[i++] = curr;
			}
			else if (curr == '_')
			{
				state = 37;
				token->lexeme[i++] = curr;
			}
			else if (curr == '#')
			{
				state = 41;
				token->lexeme[i++] = curr;
			}
			//! need to fix 44 and 46
			else if (curr == 'a' || curr >= 'e' && curr <= 'z')
			{
				state = 44;
				// token->lexeme[i++]='\0';
				token->lexeme[i++] = curr;
			}
			else if (curr >= 'b' && curr <= 'd')
			{
				state = 46;
				token->lexeme[i++] = curr;
			}
			else if ((curr - '0') >= 0 && (curr - '0') <= 9)
			{
				state = 50;
				token->lexeme[i++] = curr;
			}
			else
			{
				state = 404;
				token->lexeme[i++] = curr;
			}

			// write other cases here
			break;
		}
		case 2:
		{
			if (curr == '=')
			{
				state = 3;
				token->lexeme[i++] = curr;
			}
			else
			{
				// break into error
				state = 1;
				token->line_number = line_number;
				token->token_type = TK_ERROR;
				strcpy(token->lexeme, "Unknown symbol <=>");
				return token;
			}
			break;
		}
		case 3:
		{
			// accept state for ==
			state = 1;
			token->lexeme[i] = '\0';
			token->token_type = TK_EQ;
			token->line_number = line_number;

			return token;
		}
		case 4:
		{
			if (curr == '=')
			{
				state = 5;
				token->lexeme[i++] = curr;
			}
			else
			{
				// break into error
				state = 1;
				token->line_number = line_number;
				token->token_type = TK_ERROR;
				strcpy(token->lexeme, "Unknown symbol <!>");
				return token;
			}
			break;
		}
		case 5:
		{
			// accept state for !=
			state = 1;
			token->lexeme[i] = '\0';
			token->token_type = TK_NE;
			token->line_number = line_number;

			return token;
		}
		case 6:
		{
			// accept state for ~
			state = 1;
			token->lexeme[i] = '\0';
			token->token_type = TK_NOT;
			token->line_number = line_number;

			return token;
		}
		case 7:
		{
			if (curr == '&')
			{
				state = 8;
				token->lexeme[i++] = curr;
			}
			else
			{
				// report lexical error
				// break into error
				state = 1;
				token->line_number = line_number;
				token->token_type = TK_ERROR;
				strcpy(token->lexeme, "Unknown symbol <&>");
				return token;
			}
			break;
		}
		case 8:
		{
			if (curr == '&')
			{
				state = 9;
				token->lexeme[i++] = curr;
			}
			else
			{
				// break into error
				state = 1;
				token->line_number = line_number;
				token->token_type = TK_ERROR;
				strcpy(token->lexeme, "Unknown symbol <&&>");
				return token;
			}
			break;
		}
		case 9:
		{
			// accept state for &&&
			state = 1;
			token->lexeme[i] = '\0';
			token->token_type = TK_AND;
			token->line_number = line_number;

			return token;
		}
		case 10:
		{
			if (curr == '@')
			{
				state = 11;
				token->lexeme[i++] = curr;
			}
			else
			{
				// break into error
				state = 1;
				token->line_number = line_number;
				token->token_type = TK_ERROR;
				strcpy(token->lexeme, "Unknown symbol <@>");
				return token;
			}
			break;
		}
		case 11:
		{
			if (curr == '@')
			{
				state = 12;
				token->lexeme[i++] = curr;
			}
			else
			{
				// break into error
				state = 1;
				token->line_number = line_number;
				token->token_type = TK_ERROR;
				strcpy(token->lexeme, "Unknown symbol <@@>");
				return token;
			}
			break;
		}
		case 12:
		{
			// accept state for OR
			state = 1;
			token->lexeme[i] = '\0';
			token->token_type = TK_OR;
			token->line_number = line_number;

			return token;
		}
		case 13:
		{
			// accept state for +
			state = 1;
			token->lexeme[i] = '\0';
			token->token_type = TK_PLUS;
			token->line_number = line_number;

			return token;
		}
		case 14:
		{
			// accept state for -
			state = 1;
			token->lexeme[i] = '\0';
			token->token_type = TK_MINUS;
			token->line_number = line_number;

			return token;
		}
		case 15:
		{
			// accept state for *
			state = 1;
			token->lexeme[i] = '\0';
			token->token_type = TK_MUL;
			token->line_number = line_number;

			return token;
		}
		case 16:
		{
			// accept state for /
			state = 1;
			token->lexeme[i] = '\0';
			token->token_type = TK_DIV;
			token->line_number = line_number;

			return token;
		}
		case 17:
		{
			// accept state for .
			state = 1;
			token->lexeme[i] = '\0';
			token->token_type = TK_DOT;
			token->line_number = line_number;

			return token;
		}
		case 18:
		{
			// accept state for ,
			state = 1;
			token->lexeme[i] = '\0';
			token->token_type = TK_COMMA;
			token->line_number = line_number;

			return token;
		}
		case 19:
		{
			// accept state for :
			state = 1;
			token->lexeme[i] = '\0';
			token->token_type = TK_COLON;
			token->line_number = line_number;

			return token;
		}
		case 20:
		{
			// accept state for .
			state = 1;
			token->lexeme[i] = '\0';
			token->token_type = TK_SEM;
			token->line_number = line_number;

			return token;
		}
		case 21:
		{
			// accept state for .
			state = 1;
			token->lexeme[i] = '\0';
			token->token_type = TK_CL;
			token->line_number = line_number;

			return token;
		}
		case 22:
		{
			state = 1;
			token->lexeme[i] = '\0';
			token->token_type = TK_OP;
			token->line_number = line_number;

			return token;
		}
		case 23:
		{
			state = 1;
			token->lexeme[i] = '\0';
			token->token_type = TK_SQR;
			token->line_number = line_number;

			return token;
		}
		case 24:
		{
			state = 1;
			token->lexeme[i] = '\0';
			token->token_type = TK_SQL;
			token->line_number = line_number;

			return token;
		}
		case 25:
		{
			if (curr != '\n')
			{
				state = 25; // keep coming to this state
			}
			else
			{
				// when we found a new line
				// retract one step
				//  tb->fwd--; //to re read the \n again as single point of updation of line number
				state = 1; // re read the \n again

				strcpy(token->lexeme, "%");
				token->token_type = TK_COMMENT;
				token->line_number = line_number;
				return token;
			}
			break;
		}
		case 27:
		{
			if (curr == '=')
			{
				state = 29;
				token->lexeme[i++] = curr;
			}
			else
			{
				state = 28;
				// dont store the other character in the lexeme
			}
			break;
		}
		case 28:
		{
			// retract one step,mark the state 1 and then return the TK_GT step
			tb->fwd--;
			state = 1;

			token->lexeme[i] = '\0';
			token->token_type = TK_GT;
			token->line_number = line_number;

			return token;
		}
		case 29:
		{
			// accept state for >=
			state = 1;

			token->lexeme[i] = '\0';
			token->token_type = TK_GE;
			token->line_number = line_number;

			return token;
		}
		case 30:
		{
			// printf("in case 30");
			if (curr == '=')
			{
				state = 32;
				token->lexeme[i++] = curr;
			}
			else if (curr == '-')
			{
				state = 33;
				token->lexeme[i++] = curr;
			}
			else
			{
				// other case
				state = 31;
			}
			break;
		}
		case 31:
		{
			// retract one step
			tb->fwd--;
			state = 1;

			// also return the token
			token->lexeme[i] = '\0';
			token->token_type = TK_LT;
			token->line_number = line_number;

			return token;
		}
		case 32:
		{
			// accept state for TK_LE
			state = 1;
			token->lexeme[i] = '\0';
			token->token_type = TK_LE;
			token->line_number = line_number;

			return token;
		}
		case 33:
		{
			if (curr == '-')
			{
				token->lexeme[i++] = curr;
				state = 34;
			}
			else
			{
				// go to state 36 and then double retract from there
				state = 36;
			}
			break;
		}
		case 34:
		{
			if (curr == '-')
			{
				token->lexeme[i++] = curr;
				state = 35;
			}
			else
			{
				// report a lexical error
				// break into error
				state = 1;
				token->line_number = line_number;
				token->token_type = TK_ERROR;
				strcpy(token->lexeme, "Unknown symbol < <-- >");
				return token;
			}
			break;
		}
		case 35:
		{
			// accept state for assignop
			state = 1;

			// also remove one char from the lexeme
			token->lexeme[i] = '\0';
			token->token_type = TK_ASSIGNOP;
			token->line_number = line_number;

			return token;
		}
		case 36:
		{
			// double retract here
			tb->fwd--;
			tb->fwd--;

			state = 1;

			// also remove one char from the lexeme
			token->lexeme[--i] = '\0';
			token->token_type = TK_LT;
			token->line_number = line_number;

			return token;
		}
		case 37:
		{
			if (curr >= 'a' && curr <= 'z' || curr >= 'A' && curr <= 'Z')
			{
				state = 38;
				token->lexeme[i++] = curr;
			}
			else
			{
				//! break into error
				//! this might take some more space
				state = 1;
				token->lexeme[i] = '\0';
				token->line_number = line_number;
				token->token_type = TK_ERROR;
				char *errorMessage = malloc(sizeof(char) * 100);
				sprintf(errorMessage, "Unknown pattern <%s>", token->lexeme);
				strcpy(token->lexeme, errorMessage);
				return token;
			}
			break;
		}
		case 38:
		{
			if (curr >= 'a' && curr <= 'z' || curr >= 'A' && curr <= 'Z')
			{
				state = 38;
				token->lexeme[i++] = curr;

				//! self loop case :-
				//! goto error state when the lexeme is greater than MAX_LEXEME_SIZE
				//! as defined in the above struct
			}
			else if ((curr - '0') >= 0 && (curr - '0') <= 9)
			{
				state = 39;
				token->lexeme[i++] = curr;
			}
			else
			{
				// go here and then retract and make the token
				state = 40;
			}
			break;
		}
		case 39:
		{
			if ((curr - '0') >= 0 && (curr - '0') <= 9)
			{
				state = 39;
				token->lexeme[i++] = curr;
				//! self loop case :-
				//! goto error state when the lexeme is greater than MAX_LEXEME_SIZE
				//! as defined in the above struct
			}
			else
			{
				// go to state 40 and make the function token
				state = 40;
			}
			break;
		}
		case 40:
		{
			//! how to use the lookup functionality
			// retract one step
			tb->fwd--;
			state = 1;
			// also return the token
			//? perform a lookup here (not sure)
			token->lexeme[i] = '\0';
			token->line_number = line_number;
			if (strlen(token->lexeme) > 30)
			{
				token->token_type = TK_ERROR;
				char *errorMessage = "Function Identifier is longer than the prescribed length of 30 characters";
				strcpy(token->lexeme, errorMessage);
				return token;
			}

			Entry *e = search(token->lexeme);

			if (e != NULL)
			{
				token->token_type = e->value;
			}
			else
			{
				// also insert
				token->token_type = TK_FUNID;
				insert(token->lexeme, token->token_type);
			}
			return token;
		}
		case 41:
		{
			if (curr >= 'a' && curr <= 'z')
			{
				state = 42;
				token->lexeme[i++] = curr;
			}
			else
			{
				// break into error
				state = 1;
				token->lexeme[i] = '\0';
				token->line_number = line_number;
				token->token_type = TK_ERROR;
				char *errorMessage = malloc(sizeof(char) * 100);
				sprintf(errorMessage, "Unknown pattern <%s>", token->lexeme);
				strcpy(token->lexeme, errorMessage);
				return token;
			}
			break;
		}
		case 42:
		{
			if (curr >= 'a' && curr <= 'z')
			{
				state = 42;
				token->lexeme[i++] = curr;
			}
			else
			{
				// goto 43 and tetract
				state = 43;
			}
			break;
		}
		case 43:
		{
			tb->fwd--;
			state = 1;

			// return token record
			token->lexeme[i] = '\0';
			token->token_type = TK_RUID;
			token->line_number = line_number;

			return token;
		}
		case 44:
		{
			if (curr >= 'a' && curr <= 'z')
			{
				state = 44;
				token->lexeme[i++] = curr;
			}
			else
			{
				// goto 45 and retract
				state = 45;
			}
			break;
		}
		case 45:
		{
			tb->fwd--;
			state = 1;
			//! implement lookup functionality

			token->lexeme[i] = '\0';
			token->line_number = line_number;

			Entry *e = search(token->lexeme);

			if (e != NULL)
			{
				token->token_type = e->value;
			}
			else
			{
				// also insert
				token->token_type = TK_FIELDID;
				insert(token->lexeme, token->token_type);
			}

			return token;
		}
		case 46:
		{
			if (curr >= 'a' && curr <= 'z')
			{
				state = 44;
				token->lexeme[i++] = curr;
			}
			else if ((curr - '0') >= 2 && (curr - '0') <= 7)
			{

				state = 47;
				token->lexeme[i++] = curr;
			}
			else
			{
				state = 63; // go to 63 and retract
			}
			break;
		}
		case 63:
		{
			tb->fwd--;
			state = 1;
			//! implement lookup functionality

			token->lexeme[i] = '\0';
			token->token_type = TK_FIELDID;
			token->line_number = line_number;

			return token;
		}
		case 47:
		{
			if (curr >= 'b' && curr <= 'd')
			{
				state = 47;
				token->lexeme[i++] = curr;
			}
			else if ((curr - '0') >= 2 && (curr - '0') <= 7)
			{

				state = 48;
				token->lexeme[i++] = curr;
			}
			else
			{
				state = 49; // go to 49 and retract
			}

			break;
		}
		case 48:
		{
			if ((curr - '0') >= 2 && (curr - '0') <= 7)
			{

				state = 48;
				token->lexeme[i++] = curr;
			}
			else
			{
				state = 49; // go to 49 and retract
			}

			break;
		}
		case 49:
		{
			tb->fwd--;
			state = 1;
			//! implement lookup functionality

			token->lexeme[i] = '\0';
			token->line_number = line_number;
			if (strlen(token->lexeme) > 20)
			{
				token->token_type = TK_ERROR;
				char *errorMessage = "variable Identifier is longer than the prescribed length of 20 characters";
				strcpy(token->lexeme, errorMessage);
				return token;
			}
			token->token_type = TK_ID;

			return token;
		}

		case 50:
		{
			if ((curr - '0') >= 0 && (curr - '0') <= 9)
			{
				state = 50;
				token->lexeme[i++] = curr;
			}
			else if (curr == '.')
			{
				state = 52;
				token->lexeme[i++] = curr;
			}
			else
			{
				state = 51; // retract
			}
			break;
		}
		case 51:
		{
			tb->fwd--;
			state = 1;
			// return token record
			//! store the token value for next stage
			token->lexeme[i] = '\0';
			token->token_type = TK_NUM;
			token->line_number = line_number;

			// check if we need to fill the union tyoe
			return token;
		}
		case 52:
		{
			if ((curr - '0') >= 0 && (curr - '0') <= 9)
			{
				state = 54;
				token->lexeme[i++] = curr;
			}
			else
			{
				state = 53; // retract
			}
			break;
		}
		case 53:
		{
			// double retract here
			tb->fwd--;
			tb->fwd--;

			state = 1;
			//! store the token value for next stage
			// also remove one char from the lexeme
			token->lexeme[--i] = '\0';
			token->token_type = TK_NUM;
			token->line_number = line_number;

			return token;
		}
		case 54:
		{
			if ((curr - '0') >= 0 && (curr - '0') <= 9)
			{
				state = 55;
				token->lexeme[i++] = curr;
			}
			else
			{
				state = 1;
				token->lexeme[i] = '\0';
				token->line_number = line_number;
				token->token_type = TK_ERROR;
				char *errorMessage = malloc(sizeof(char) * 100);
				sprintf(errorMessage, "Unknown pattern <%s>", token->lexeme);
				strcpy(token->lexeme, errorMessage);
				return token;
			}
			break;
		}
		case 55:
		{
			if (curr == 'E')
			{
				state = 57;
				token->lexeme[i++] = curr;
			}
			else
			{
				state = 56; // retract
			}
			break;
		}
		case 56:
		{
			tb->fwd--;
			state = 1;
			//! store the token value for next stage
			token->lexeme[i] = '\0';
			token->token_type = TK_RNUM;
			token->line_number = line_number;

			return token;
		}
		case 57:
		{
			if (curr == '+' || curr == '-')
			{
				state = 58;
				token->lexeme[i++] = curr;
			}
			else if ((curr - '0') >= 0 && (curr - '0') <= 9)
			{
				state = 59;
				token->lexeme[i++] = curr;
			}
			else
			{
				// report a lexical error here
				state = 1;
				token->lexeme[i] = '\0';
				token->line_number = line_number;
				token->token_type = TK_ERROR;
				char *errorMessage = malloc(sizeof(char) * 100);
				sprintf(errorMessage, "Unknown pattern <%s>", token->lexeme);
				strcpy(token->lexeme, errorMessage);
				return token;
			}
			break;
		}
		case 58:
		{
			if ((curr - '0') >= 0 && (curr - '0') <= 9)
			{
				state = 59;
				token->lexeme[i++] = curr;
			}
			else
			{
				// report a lexical error here
				state = 1;
				token->lexeme[i] = '\0';
				token->line_number = line_number;
				token->token_type = TK_ERROR;
				char *errorMessage = malloc(sizeof(char) * 100);
				sprintf(errorMessage, "Unknown pattern <%s>", token->lexeme);
				strcpy(token->lexeme, errorMessage);
				return token;
			}
			break;
		}
		case 59:
		{
			if ((curr - '0') >= 0 && (curr - '0') <= 9)
			{
				state = 60;
				token->lexeme[i++] = curr;
			}
			else
			{
				// report a lexical error here
				state = 1;
				token->lexeme[i] = '\0';
				token->line_number = line_number;
				token->token_type = TK_ERROR;
				char *errorMessage = malloc(sizeof(char) * 100);
				sprintf(errorMessage, "Unknown pattern <%s>", token->lexeme);
				strcpy(token->lexeme, errorMessage);
				return token;
			}
			break;
		}
		case 60:
		{
			//! if we get a number here do we have to report a lex error ?
			if (((curr - '0') >= 0 && (curr - '0') <= 9) == 0)
			{
				state = 61;
			}
			else
			{
				//? error this
				state = 61;
			}
			break;
		}
		case 61:
		{
			tb->fwd--;
			state = 1;

			token->lexeme[i] = '\0';
			token->token_type = TK_RNUM;
			token->line_number = line_number;

			return token;
		}

		case 404:
		{
			// report a lexical error here
			state = 1;
			token->lexeme[i] = '\0';
			token->line_number = line_number;
			token->token_type = TK_ERROR;
			char *errorMessage = malloc(sizeof(char) * 100);
			sprintf(errorMessage, "Unknown pattern <%s>", token->lexeme);
			strcpy(token->lexeme, errorMessage);
			return token;
		}

		default:
		{
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
		// at the end it is by default forwading
		tb->fwd++;

		// printf("the fwd is pointing to %c\n", curr);
		checkAndLoadBuffer(tb);
	}

	// if we are returning from here then token has an error
	//  strcpy(token->lexeme, "error");
	return NULL;
}

// int main()
// {
// 	TwinBuffer twinBuffer;
// 	FILE *inputFile = fopen("Test Cases/t1.txt", "r");
// 	if (inputFile == NULL)
// 	{
// 		fprintf(stderr, "Error opening input file.\n");
// 		return 1;
// 	}

// 	if (setupLexer(&twinBuffer, inputFile) != 0)
// 	{
// 		fprintf(stderr, "Error setting up lexer.\n");
// 		fclose(inputFile);
// 		return 1;
// 	}
// 	AddtoHashTable();
// 	parser_input_head = (token_input *)malloc(sizeof(token_input));

// // 	populateTerminals("terminals.txt");
// // 	populateNonTerminals("non_terminals.txt");
// // 	fill_epsilon();
// // 	readGrammar("grammar.txt");
// // 	populateFirst();
// // 	epsilonFirst();
// // 	populateFollow();
// // 	initializeErrorTokens();
// // 	fillMatrix();

// 	token_input *curr = parser_input_head;
// 	st = initializeStack();
// 	root = createTreeNode(non_terminal_array[0]);
// 	TreeNode *dollarnode = createTreeNode(terminal_array[0]);
// 	root->nextSibling = dollarnode;
// 	push(st, terminal_array[0]);
// 	push(st, non_terminal_array[0]);
// 	curr_child = root;
// 	while (true)
// 	{
// 		TOKEN *token = getNextToken(&twinBuffer);
// 		if (token->isEOF)
// 		{
// 			break;
// 		}
// 		if (token->token_type == TK_ERROR)
// 		{
// 			printf(RED "Line Number: %u Error: %s \n", token->line_number, token->lexeme);
// 		}
// 		printToken(token);
// 		if (token->token_type != TK_ERROR && token->token_type != TK_COMMENT)
// 		{
// 			token_input *new_token = (token_input *)malloc(sizeof(token_input));
// 			new_token->linenum = token->line_number;
// 			strcpy(new_token->name, token_type_to_string(token->token_type));
// 			strcpy(new_token->value, token->lexeme);
// 			new_token->varNum = assignNumToTokens(new_token);
// 			// curr = new_token;
// 			// parser(new_token);
// 		}
// 	}
// 	// int i = 1;

// 	// while (curr != NULL)
// 	// {
// 	// 	printf("%d. %s\n", i, curr->name);
// 	// 	i++;
// 	// 	curr = curr->next_token;
// 	// }

// 	// invoke your lexer and parser here

// 	// printTree(root);

// 	for (int i = 0; i < TERMINALS; i++)
// 	{
// 		free(terminal_array[i]);
// 	}
// 	for (int i = 0; i < NON_TERMINALS; i++)
// 	{
// 		free(non_terminal_array[i]);
// 	}

// 	fclose(inputFile);

// 	return 0;
// }

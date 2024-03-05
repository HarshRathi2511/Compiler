/*  Group No : 7

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "lexer.c"
#include "lexerDef.h"
#include "colorCodes.h"
#include "firstandfollow.h"
#include "stack_ADT.h"
#include "tree_ADT.h"
#include "lexer.h"
#include <time.h>
int main()
{
	TwinBuffer twinBuffer;

	char *inputFileChar = "t1.txt";
	char *outputFileChar = "output.txt";

	AddtoHashTable();

	int input;
	do
	{

		clock_t start_time, end_time;

		double total_CPU_time, total_CPU_time_in_seconds;

		start_time = clock();

		printf("\n\n###### CHOOSE AN OPTION ###### \n\n");
		printf("0: EXIT\n");
		printf("1: Remove comments from the testcase file\n");
		printf("2: Run lexer, Generate and Print token list on console\n");
		printf("3: Run Parser, Print all errors on the console  and print Parse Tree in a file \n");
		printf("4: Total Compiling Time\n");
		printf("Enter Your Choice : ");
		// global_vars(); // Initialization of the global variables
		scanf("%d", &input);
		printf("\n");
		switch (input)
		{
		case 0:
		{
			printf("\nSuccessfully EXITED\n");
			break;
		}

		case 1:
		{
			Remove_Comments(inputFileChar, outputFileChar);
			printf("ALL THE COMMENTS IN THE TESTCASE FILE ARE SUCESSFULLY REMOVED.\n");
			// print the time for printing the tokens
			break;
		}

		case 2:
		{

			FILE *inputFile = fopen(inputFileChar, "r");
			FILE *outputFile = fopen(outputFileChar, "w"); // Open file for writing tokens
			if (inputFile == NULL || outputFile == NULL)
			{
				fprintf(stderr, "Error opening input or output file.\n");
				return 1;
			}
			// Remove_Comments(inputFile, outputFile);
			//         printf("ALL THE COMMENTS IN THE TESTCASE FILE ARE SUCESSFULLY REMOVED.\n");

			if (setupLexer(&twinBuffer, inputFile) != 0)
			{
				fprintf(stderr, "Error setting up lexer.\n");
				fclose(inputFile);
				fclose(outputFile);
				return 1;
			}
			while (true)
			{
				TOKEN *token = getNextToken(&twinBuffer);
				if (token->isEOF)
				{
					break;
				}
				printToken(token);
			}

			fclose(inputFile);
			fclose(outputFile);

			// for new instance of running the lexer
			//! add the global variables here
			// line_number = 1;
			// state = 1;

			break;
		}
		case 3:
		{
			FILE *inputFile = fopen(inputFileChar, "r");
			FILE *outputFile = fopen(outputFileChar, "w"); // Open file for writing tokens
			if (inputFile == NULL || outputFile == NULL)
			{
				fprintf(stderr, "Error opening input or output file.\n");
				return 1;
			}
			// Remove_Comments(inputFile, outputFile);
			//         printf("ALL THE COMMENTS IN THE TESTCASE FILE ARE SUCESSFULLY REMOVED.\n");

			if (setupLexer(&twinBuffer, inputFile) != 0)
			{
				fprintf(stderr, "Error setting up lexer.\n");
				fclose(inputFile);
				fclose(outputFile);
				return 1;
			}
			populateTerminals("terminals.txt");
			populateNonTerminals("non_terminals.txt");
			fill_epsilon();
			readGrammar("grammar.txt");
			populateFirst();
			epsilonFirst();
			populateFollow();
			initializeErrorTokens();
			fillMatrix();
			parser_input_head = (token_input *)malloc(sizeof(token_input));
			token_input *curr = parser_input_head;
			st = initializeStack();
			root = createTreeNode(non_terminal_array[0]);
			TreeNode *dollarnode = createTreeNode(terminal_array[0]);
			root->nextSibling = dollarnode;
			push(st, terminal_array[0]);
			push(st, non_terminal_array[0]);
			curr_child = root;
			while (true)
			{
				TOKEN *token = getNextToken(&twinBuffer);
				if (token->isEOF)
				{
					break;
				}
				if (token->token_type == TK_ERROR)
				{
					printf(RED "Line Number: %u Error: %s \n", token->line_number, token->lexeme);
				}

				if (token->token_type != TK_ERROR && token->token_type != TK_COMMENT)
				{
					token_input *new_token = (token_input *)malloc(sizeof(token_input));
					new_token->linenum = token->line_number;
					strcpy(new_token->name, token_type_to_string(token->token_type));
					strcpy(new_token->value, token->lexeme);
					new_token->varNum = assignNumToTokens(new_token);
					// curr = new_token;
					parser(new_token);
				}
			}
			//! call print here
			// printTree(root, 0);

			fclose(inputFile);
			fclose(outputFile);
		}

		case 4:
		{
			clock_t start, end;
			double cpu_time_used;
			start = clock();
			// Your code here
			end = clock();
			cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
			printf("Total time taken by CPU: %f\n", cpu_time_used);
			break;
		}
		default:
			printf("\nResponse %d is not valid, please enter the appropriate input\n", input);
		}
		// fclose(file_ptr);
	} while (input != 0);
	// return 0;

	return 0;
}

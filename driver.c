/*  Group No : 7
2020B1A71605P Samarth Trivedi
2020B1A71612P Harshil Sethi
2020B1A70644P Nitish Sahani
2020B3A70786P Neel Dedhia
2020B3A70794P Harsh Rathi
2020B5A70604P Divyan Poddar
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
int main(int argc, char *argv[])
{
	TwinBuffer twinBuffer;

	char *inputFileChar;
	char *outputFileChar;

	// Check if the correct number of arguments are provided
	if (argc != 3)
	{
		printf("Usage: %s <inputFile> <outputFile>\n", argv[0]);
		return 1;
	}

	inputFileChar = argv[1];
	outputFileChar = argv[2];

	// Check if input file name is empty
	if (strlen(inputFileChar) == 0)
	{
		printf("Error: Input file name cannot be empty\n");
		return 1;
	}

	// Check if output file name is empty
	if (strlen(outputFileChar) == 0)
	{
		printf(RED "Error: Output file name cannot be empty\n");
		return 1;
	}

	// Check format of input files
	if (strstr(inputFileChar, ".txt") == NULL || strstr(outputFileChar, ".txt") == NULL)
	{
		printf(RED "Error: Files should be of .txt format\n");
		return 1;
	}

	AddtoHashTable();
	populateTerminals("terminals.txt");
	populateNonTerminals("non_terminals.txt");
	fill_epsilon();
	readGrammar("grammar.txt");
	populateFirst();
	epsilonFirst();
	populateFollow();
	initializeErrorTokens();
	fillMatrix();

	int input;
	printf("THE FOLLOWING IMPLEMENTATIONS HAVE BEEN COMPLETED\n");
	printf("1) FIRST and FOLLOW set automated\n");
	printf("2) Both lexical and syntax analysis modules implemented\n");
	printf("3) The parse tree has been constructed\n");
	printf("4) All modules work with all the given 1-6 testcases\n");
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
			freeAllafterMenu();
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
			resetStateAndLineNumber();

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
			initializeStackandTree();

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
				// printToken(token);
				if (token->token_type != TK_ERROR && token->token_type != TK_COMMENT)
				{
					token_input *new_token = (token_input *)malloc(sizeof(token_input));
					new_token->linenum = token->line_number;
					strcpy(new_token->name, token_type_to_string(token->token_type));
					strcpy(new_token->value, token->lexeme);
					new_token->varNum = assignNumToTokens(new_token);
					// curr = new_token;
					parser(new_token);
					free(new_token);
				}
				free(token);
			}

			printparsingtree();
			freeAllafterParsing();
			fclose(inputFile);
			fclose(outputFile);

			// reset
			resetStateAndLineNumber();
			break;
		}

		case 4:
		{
			clock_t start, end;
			double cpu_time_used;
			start = clock();
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
			initializeStackandTree();

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
				// printToken(token);
				if (token->token_type != TK_ERROR && token->token_type != TK_COMMENT)
				{
					token_input *new_token = (token_input *)malloc(sizeof(token_input));
					new_token->linenum = token->line_number;
					strcpy(new_token->name, token_type_to_string(token->token_type));
					strcpy(new_token->value, token->lexeme);
					new_token->varNum = assignNumToTokens(new_token);
					// curr = new_token;
					parser(new_token);
					free(new_token);
				}
				free(token);
			}

			printparsingtree();
			freeAllafterParsing();
			fclose(inputFile);
			fclose(outputFile);

			// reset
			resetStateAndLineNumber();
			// Your code here
			end = clock();
			cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
			unsigned long long int total_clock_cycles;
			total_clock_cycles = end - start; // Calculate the total clock cycles

			printf("Total time taken by CPU: %f\n", cpu_time_used);
			printf("Total CPU clock cycles: %llu\n", total_clock_cycles);
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

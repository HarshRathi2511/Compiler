/*  Group No : 7

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.c"
#include "lexerDef.h"
// #include "lexer.h"
#include <time.h>
int main()
{
	TwinBuffer twinBuffer;
	char *inputFileChar = "t1.txt";
	char *outputFileChar = "output.txt";
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

	AddtoHashTable();

	int input;
	do
	{
		// global_vars(); // Initialization of the global variables

		printf("\n\n###### CHOOSE AN OPTION ###### \n\n");
		printf("0: EXIT\n");
		printf("1: Remove comments from the testcase file\n");
		printf("2: Run lexer, Generate and Print token list on console\n");
		printf("3: Run Parser, Print all errors on the console  and print Parse Tree in a file \n");
		printf("4: Total Compiling Time\n");
		printf("Enter Your Choice : ");
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
			break;
		}

		case 2:
		{

			while (true)
			{
				TOKEN *token = getNextToken(&twinBuffer);
				if (token->isEOF)
				{
					break;
				}
				printToken(token);
			}
			break;
		}
		case 3: {

		}
		
		case 4:{
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

	fclose(inputFile);
	fclose(outputFile);

	return 0;
}

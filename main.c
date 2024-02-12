#include <stdio.h>
#include <stdlib.h>
#include "twin_buffer.h"

int main()
{
	TwinBuffer twin_buffer;
	const char *filename = "code.txt";
	initialize_twin_buffer(&twin_buffer, filename); //call this function to initialize the twin buffer

	char next_char;
	//twin_buffer_get_next_char :- call this function from the lexer
	while ((next_char = twin_buffer_get_next_char(&twin_buffer)) != EOF)
	{
		putchar(next_char); // prints the character read from the file to the console
		printf("...");
		// or perform any other operation with the character
		// Check if the next character is a whitespace character
		if (next_char == ' ' || next_char == '\t' || next_char == '\n' || next_char == '\r')
		{
			// printf("\nWhitespace detected!\n");
		}
	}

	fclose(twin_buffer.file);
	return 0;
}
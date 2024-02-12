#include <stdio.h>
#include <stdlib.h>

#define TWIN_BUFFER_SIZE 1024

typedef struct
{
	char buffer1[TWIN_BUFFER_SIZE];
	char buffer2[TWIN_BUFFER_SIZE];
	char *forward;
	FILE *file;
} TwinBuffer;

// Function prototypes
// void reload_buffer(TwinBuffer *twin_buffer, char *buffer);
void initialize_twin_buffer(TwinBuffer *twin_buffer, const char *filename);
char twin_buffer_get_next_char(TwinBuffer *twin_buffer);
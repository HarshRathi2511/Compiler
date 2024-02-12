#include "twin_buffer.h"

void reload_buffer(TwinBuffer *twin_buffer, char *buffer)
{
	size_t bytes_read = fread(buffer, 1, TWIN_BUFFER_SIZE, twin_buffer->file);
	if (bytes_read < TWIN_BUFFER_SIZE)
	{
		// Reached end of file or error occurred
		buffer[bytes_read] = '\0'; // Null-terminate the buffer
	}
}

void initialize_twin_buffer(TwinBuffer *twin_buffer, const char *filename)
{
	twin_buffer->file = fopen(filename, "r");
	if (twin_buffer->file == NULL)
	{
		fprintf(stderr, "Error opening file: %s\n", filename);
		exit(1);
	}
	reload_buffer(twin_buffer, twin_buffer->buffer1); // initia
	twin_buffer->forward = twin_buffer->buffer1;
}

char twin_buffer_get_next_char(TwinBuffer *twin_buffer)
{
	if (twin_buffer->forward == twin_buffer->buffer1 + TWIN_BUFFER_SIZE)
	{
		reload_buffer(twin_buffer, twin_buffer->buffer2);
		twin_buffer->forward = twin_buffer->buffer2;
	}
	else if (twin_buffer->forward == twin_buffer->buffer2 + TWIN_BUFFER_SIZE)
	{
		reload_buffer(twin_buffer, twin_buffer->buffer1);
		twin_buffer->forward = twin_buffer->buffer1;
	}
	else if (*twin_buffer->forward == '\0')
	{
		// If end-of-file or end-of-buffer is reached, return EOF
		return EOF; // which is -1
	}
	return *(twin_buffer->forward)++;
}

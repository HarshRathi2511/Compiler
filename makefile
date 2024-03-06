# Define compiler
CC=gcc
# Define compiler flags
CFLAGS=-c

# Define source files
SOURCES=stack_ADT.c tree_ADT.c parser.c lexer.c driver.c

# Define object files
OBJECTS=$(SOURCES:.c=.o)

# Define executable
EXECUTABLE=stage1exe

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

# Compile each source file into object files
%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

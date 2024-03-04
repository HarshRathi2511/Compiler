CC = gcc
CFLAGS = -Wall -Wextra -g  # Add any additional compiler flags here

# List of source files
SRCS = stack_ADT.c tree_ADT.c firstandfollow.c lexer.c
OBJS = $(SRCS:.c=.o)

# Executable name
EXECUTABLE = my_program

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
    $(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -f $(OBJS) $(EXECUTABLE)

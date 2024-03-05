#!/bin/bash

# Compile source files
gcc -c stack_ADT.c -o stack_ADT.o
gcc -c tree_ADT.c -o tree_ADT.o
gcc -c firstandfollow.c -o firstandfollow.o
gcc -c lexer.c -o lexer.o

# Link object files to create executable
gcc stack_ADT.o tree_ADT.o lexer.o firstandfollow.o -o program

# Run the executable
# ./program

# rm *.o

#!/bin/bash

# Compile source files
gcc -c stack_ADT.c -o stack_ADT.o
gcc -c tree_ADT.c -o tree_ADT.o
gcc -c parser.c -o parser.o
gcc -c lexer.c -o lexer.o
gcc -c driver.c -o driver.o

# Link object files to create executable
gcc stack_ADT.o tree_ADT.o lexer.o parser.o driver.o -o program

# Run the executable
# ./program

# rm *.o

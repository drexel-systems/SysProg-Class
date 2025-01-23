# Makefile for compiling demonstration C files

# Compiler
CC = gcc

# Standard compiler flags
CFLAGS =  -g

# Source files
SOURCES = mem-layout.c
# Executable names
EXECUTABLES = $(SOURCES:.c=)

# Default target: build all executables
all: $(EXECUTABLES) 

# Rule for standard compilation
$(EXECUTABLES): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

# Clean up compiled files
clean:
	rm -f $(EXECUTABLES) 

# Phony targets
.PHONY: all clean
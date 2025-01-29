# Makefile for compiling demonstration C files

# Compiler
CC = gcc

# Standard compiler flags
CFLAGS = -Wall -Wextra 

# Source files
STD_SOURCES = ret21.c ret22.c ret23.c
NO_START_SOURCES = start24.c start25.c start26.c start27.c start28.c

# All source files
SOURCES = $(STD_SOURCES) $(NO_START_SOURCES)

# Executable names
STD_EXECUTABLES = $(STD_SOURCES:.c=)
NO_START_EXECUTABLES = $(NO_START_SOURCES:.c=)

# Default target: build all executables
all: $(STD_EXECUTABLES) $(NO_START_EXECUTABLES)

# Rule for standard compilation
$(STD_EXECUTABLES): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

# Rule for files requiring -nostartfiles
$(NO_START_EXECUTABLES): %: %.c
	$(CC) $(CFLAGS) -nostartfiles -o $@ $<

# Clean up compiled files
clean:
	rm -f $(STD_EXECUTABLES) $(NO_START_EXECUTABLES)

# Phony targets
.PHONY: all clean
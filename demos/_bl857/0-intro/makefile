# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g

# Target executable name
TARGET = $(patsubst %.c,%,$(wildcard [0-9]*.c))

# Default target
all: $(TARGET)

# Compile source to executable, object file, assembler file, and preprocessed file
%: %.c
	$(CC) $(CFLAGS) -o $@ $<
	$(CC) $(CFLAGS) -c $< -o $@.o
	$(CC) $(CFLAGS) -S $< -o $@.s
	$(CC) $(CFLAGS) -E $< -o $@.i

# Clean up build files
clean:
	rm -f $(TARGET) $(TARGET:=.o) $(TARGET:=.s) $(TARGET:=.i)

# Phony targets
.PHONY: all clean
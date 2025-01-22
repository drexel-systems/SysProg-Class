# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g

# Target executable name
TARGET = $(patsubst %.c,%,$(wildcard [0-9]*.c))

# Default target
all: $(TARGET) .gitignore

# Compile source to executable, object file, assembler file, and preprocessed file
%: %.c
	@$(CC) $(CFLAGS) -o $@ $<
	@$(CC) $(CFLAGS) -S -o $@.s $<

# Generate .gitignore
.gitignore: $(TARGET)
	@echo $(TARGET) | tr ' ' '\n' > .gitignore

# Clean up build files
clean:
	rm -f $(TARGET) $(TARGET:=.o) $(TARGET:=.s) $(TARGET:=.i) .gitignore

# Phony targets
.PHONY: all clean .gitignore
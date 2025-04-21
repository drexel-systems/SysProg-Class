# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g

# Target executable name
TARGET = stringfun

# Default target
all: $(TARGET)

test: $(TARGET)
	bats $(wildcard ./bats/*.sh)
	
# Compile source to executable
$(TARGET): stringfun.c
	$(CC) $(CFLAGS) -o $(TARGET) $^

# Clean up build files
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean
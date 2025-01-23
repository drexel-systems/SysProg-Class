# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g

# Target executable name
TARGET = sdbsc

# Find all source and header files
SRCS = $(wildcard *.c)
HDRS = $(wildcard *.h)

# Default target
all: $(TARGET)

# Compile source to executable
$(TARGET): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Clean up build files
clean:
	rm -f $(TARGET)
	rm -f student.db

test:
	./test.sh

# Phony targets
.PHONY: all clean
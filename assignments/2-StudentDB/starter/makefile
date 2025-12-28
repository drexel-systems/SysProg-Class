# Makefile for Simple Database Assignment

CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = sdbsc
SRC = sdbsc.c
TEST_SCRIPT = test_sdbsc.py

# Default target - compile directly without intermediate .o files
all: $(TARGET)

# Build the executable directly from source
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Run tests using pytest
test: $(TARGET)
	@echo "Running pytest tests..."
	@pytest $(TEST_SCRIPT) -v

# Run tests with more detailed output
test-verbose: $(TARGET)
	@echo "Running pytest tests with detailed output..."
	@pytest $(TEST_SCRIPT) -vv

# Run specific test
test-one: $(TARGET)
	@echo "Run a specific test with: make test-one TEST=test_name"
	@pytest $(TEST_SCRIPT) -v -k "$(TEST)"

# Clean build artifacts
clean:
	rm -f $(TARGET) *.o student.db

# Clean and rebuild
rebuild: clean all

# Install pytest if not present (requires pip)
install-pytest:
	@echo "Installing pytest..."
	pip3 install pytest --break-system-packages

.PHONY: all test test-verbose test-one clean rebuild install-pytest

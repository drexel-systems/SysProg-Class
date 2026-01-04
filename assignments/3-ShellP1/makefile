# Makefile for dsh (Drexel Shell) Part 1

CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = dsh
SRC = dsh_cli.c dshlib.c
HDRS = dshlib.h
TEST_SCRIPT = test_dsh.py

# Default target - build the shell
all: $(TARGET)

# Build the executable directly from sources
$(TARGET): $(SRC) $(HDRS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Run tests using pytest
test: $(TARGET)
	@echo "Running pytest tests..."
	@pytest $(TEST_SCRIPT) -v

# Run tests with more detailed output
test-verbose: $(TARGET)
	@echo "Running pytest tests with detailed output..."
	@pytest $(TEST_SCRIPT) -vv --tb=long

# Run specific test class
test-class: $(TARGET)
	@echo "Run specific test class with: make test-class CLASS=TestSingleCommands"
	@pytest $(TEST_SCRIPT)::$(CLASS) -v

# Run specific test function
test-one: $(TARGET)
	@echo "Run specific test with: make test-one TEST=test_single_command_no_args"
	@pytest $(TEST_SCRIPT) -v -k "$(TEST)"

# Run valgrind memory check
valgrind: $(TARGET)
	@echo "Running valgrind memory check..."
	@echo -e "cmd\nexit" | valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./$(TARGET)

# Run valgrind with helgrind (thread checker - for future assignments)
helgrind: $(TARGET)
	@echo "Running helgrind thread checker..."
	@echo -e "cmd\nexit" | valgrind --tool=helgrind --error-exitcode=1 ./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET) *.o

# Clean and rebuild
rebuild: clean all

# Install pytest if not present
install-pytest:
	@echo "Installing pytest..."
	pip3 install pytest --break-system-packages

# Help target
help:
	@echo "dsh (Drexel Shell) Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  all            - Build dsh (default)"
	@echo "  test           - Run all pytest tests"
	@echo "  test-verbose   - Run tests with detailed output"
	@echo "  test-class     - Run specific test class: make test-class CLASS=TestSingleCommands"
	@echo "  test-one       - Run specific test: make test-one TEST=test_name"
	@echo "  valgrind       - Run memory leak check"
	@echo "  helgrind       - Run thread checker (for future assignments)"
	@echo "  clean          - Remove built files"
	@echo "  rebuild        - Clean and rebuild"
	@echo "  install-pytest - Install pytest"
	@echo "  help           - Show this help"

.PHONY: all test test-verbose test-class test-one valgrind helgrind clean rebuild install-pytest help

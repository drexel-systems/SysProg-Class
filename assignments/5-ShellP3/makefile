# Makefile for dsh (Drexel Shell) Part 3

CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = dsh
SRC = dsh_cli.c dshlib.c
HDRS = dshlib.h
TEST_SCRIPT = test_dsh3.py

# Optional: include dragon.c if implemented
# SRC += dragon.c

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
	@echo "Run specific test class with: make test-class CLASS=TestTwoCommandPipes"
	@pytest $(TEST_SCRIPT)::$(CLASS) -v

# Run specific test function
test-one: $(TARGET)
	@echo "Run specific test with: make test-one TEST=test_ls_pipe_cat"
	@pytest $(TEST_SCRIPT) -v -k "$(TEST)"

# Run strace on shell to see pipe/dup2 operations
strace-pipes: $(TARGET)
	@echo "Running strace on dsh to see pipe operations..."
	@echo "Commands to try: ls | cat, echo hello | cat, exit"
	@echo ""
	strace -f -e trace=pipe,dup2,close,fork,execve ./$(TARGET)

# Trace just pipe and dup2 (cleaner output)
strace-clean: $(TARGET)
	@echo "Running strace with clean output (pipe,dup2,close only)..."
	@echo "Commands to try: ls | cat, exit"
	@echo ""
	strace -f -e trace=pipe,dup2,close ./$(TARGET)

# Save strace output to file
strace-save: $(TARGET)
	@echo "Running strace and saving to trace.txt..."
	@echo "Commands to try: ls | cat, echo hello | cat, exit"
	@echo ""
	strace -f -e trace=pipe,dup2,close,fork,execve -o trace.txt ./$(TARGET)
	@echo ""
	@echo "Output saved to trace.txt"

# Run valgrind memory check
valgrind: $(TARGET)
	@echo "Running valgrind memory check..."
	@echo -e "echo hello | cat\nexit" | valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./$(TARGET)

# Run valgrind with file descriptor tracking
valgrind-fds: $(TARGET)
	@echo "Running valgrind with file descriptor tracking..."
	@echo -e "echo hello | cat\nexit" | valgrind --track-fds=yes --leak-check=full ./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET) *.o trace.txt

# Clean and rebuild
rebuild: clean all

# Install pytest if not present
install-pytest:
	@echo "Installing pytest..."
	pip3 install pytest --break-system-packages

# Help target
help:
	@echo "dsh (Drexel Shell) Part 3 Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  all            - Build dsh (default)"
	@echo "  test           - Run all pytest tests"
	@echo "  test-verbose   - Run tests with detailed output"
	@echo "  test-class     - Run specific test class: make test-class CLASS=TestTwoCommandPipes"
	@echo "  test-one       - Run specific test: make test-one TEST=test_name"
	@echo "  strace-pipes   - Run dsh with strace to see pipe/dup2/fork syscalls"
	@echo "  strace-clean   - Run strace with cleaner output (pipe,dup2,close only)"
	@echo "  strace-save    - Run strace and save output to trace.txt"
	@echo "  valgrind       - Run memory leak check"
	@echo "  valgrind-fds   - Run valgrind with file descriptor tracking"
	@echo "  clean          - Remove built files"
	@echo "  rebuild        - Clean and rebuild"
	@echo "  install-pytest - Install pytest"
	@echo "  help           - Show this help"
	@echo ""
	@echo "Useful for strace analysis (Part 3 assignment):"
	@echo "  make strace-pipes    # See all pipe operations"
	@echo "  make strace-clean    # Cleaner output"
	@echo "  make strace-save     # Save to file for analysis"
	@echo ""
	@echo "Manual strace examples:"
	@echo "  strace -f -e trace=pipe,dup2,close ./dsh"
	@echo "  strace -f -e trace=pipe,dup2 ./dsh 2>&1 | grep -E '(pipe|dup2)'"

.PHONY: all test test-verbose test-class test-one strace-pipes strace-clean strace-save valgrind valgrind-fds clean rebuild install-pytest help

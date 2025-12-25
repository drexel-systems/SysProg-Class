CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11
TARGET = minigrep
SOURCE = minigrep.c

# Default target - compile directly from source to executable
all: $(TARGET)

# Build the executable directly (no .o files)
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Run tests using pytest (recommended)
test: $(TARGET)
	@echo "Running tests with pytest..."
	@if command -v pytest >/dev/null 2>&1; then \
		pytest test_minigrep.py -v; \
	else \
		echo "pytest not installed. Install with: pip install pytest"; \
		echo "Falling back to BATS..."; \
		if command -v bats >/dev/null 2>&1; then \
			bats test.sh; \
		else \
			echo "Neither pytest nor BATS installed."; \
		fi \
	fi

# Run tests with BATS (legacy support)
test-bats: $(TARGET)
	@if command -v bats >/dev/null 2>&1; then \
		bats test.sh; \
	else \
		echo "BATS not installed. Install with: sudo apt-get install bats"; \
	fi

# Run tests with pytest (explicit)
test-pytest: $(TARGET)
	@if command -v pytest >/dev/null 2>&1; then \
		pytest test_minigrep.py -v; \
	else \
		echo "pytest not installed. Install with: pip install pytest"; \
	fi

# Clean build artifacts
clean:
	rm -f $(TARGET) *.o

# Create a sample test file for manual testing
sample:
	@echo "Creating sample test files..."
	@echo "This is a test file" > test_data.txt
	@echo "It contains multiple lines" >> test_data.txt
	@echo "Some lines have ERROR messages" >> test_data.txt
	@echo "Other lines have warnings" >> test_data.txt
	@echo "This line is fine" >> test_data.txt
	@echo "ERROR: Something went wrong" >> test_data.txt
	@echo "Another test line here" >> test_data.txt
	@echo "Sample files created: test_data.txt"

# Run program with sample input
demo: $(TARGET) sample
	@echo "\n=== Demo: Basic search ==="
	./$(TARGET) "test" test_data.txt
	@echo "\n=== Demo: Search with line numbers ==="
	./$(TARGET) -n "ERROR" test_data.txt
	@echo "\n=== Demo: Case-insensitive search ==="
	./$(TARGET) -i "error" test_data.txt
	@echo "\n=== Demo: Count matches ==="
	./$(TARGET) -c "line" test_data.txt

.PHONY: all test test-bats test-pytest clean sample demo

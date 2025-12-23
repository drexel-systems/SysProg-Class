# Pytest Tutorial - Quick Start Guide

## What is Pytest?

Pytest is a Python testing framework that makes it easy to write and run automated tests. In this course, we use pytest to verify that your C programs produce correct output.

## Why Use Pytest for C Programs?

Your C programs are command-line tools that:
- Accept input via stdin or files
- Produce output to stdout
- Return exit codes

Pytest lets us automatically test all of these behaviors by:
1. Running your compiled binary with different inputs
2. Capturing the output
3. Verifying it matches expected results

This means you can quickly verify your program works correctly across dozens of test cases instead of manually testing each one.

## How Tests Work

### Basic Structure

```python
def test_something():
    # 1. Arrange: Set up test data
    input_data = "test input"
    
    # 2. Act: Run your program
    result = subprocess.run(["./wordcount"], 
                          input=input_data,
                          capture_output=True,
                          text=True)
    
    # 3. Assert: Check the results
    assert result.returncode == 0
    assert "expected output" in result.stdout
```

### Real Example from test_wordcount.py

```python
def test_lines_only(self):
    """Test -l option (lines only)"""
    input_text = "Line 1\nLine 2\nLine 3\n"
    result = subprocess.run(
        [BINARY, "-l"],
        input=input_text,
        capture_output=True,
        text=True
    )
    
    assert result.returncode == 0
    output = result.stdout.strip()
    parts = output.split()
    assert len(parts) == 1
    assert parts[0] == "3"
```

**What this test does:**
1. Creates input with 3 lines
2. Runs `./wordcount -l` with that input
3. Captures the output
4. Verifies the program returns success (exit code 0)
5. Checks that the output is exactly "3" (three lines counted)

## Key Pytest Concepts

### Assertions
The `assert` statement checks if something is true. If it fails, the test fails:
```python
assert parts[0] == "3"  # PASS if parts[0] is "3", FAIL otherwise
```

### Test Discovery
Pytest automatically finds and runs:
- Files named `test_*.py` or `*_test.py`
- Functions named `test_*()`
- Classes named `Test*`

### Fixtures
Fixtures set up reusable test data:
```python
@pytest.fixture
def sample_file(tmp_path):
    """Create a temporary test file"""
    file = tmp_path / "test.txt"
    file.write_text("Hello world\n")
    return file

def test_file_input(sample_file):
    # sample_file is automatically created and passed in
    result = subprocess.run([BINARY, str(sample_file)])
```

## Running Tests

```bash
# Run all tests
pytest test_wordcount.py

# Run with verbose output (shows each test name)
pytest test_wordcount.py -v

# Run a specific test
pytest test_wordcount.py::TestOptions::test_lines_only

# Show print statements (useful for debugging)
pytest test_wordcount.py -v -s
```

## Reading Test Output

**When a test passes:**
```
test_wordcount.py::TestOptions::test_lines_only PASSED [25%]
```

**When a test fails:**
```
test_wordcount.py::TestOptions::test_lines_only FAILED [25%]

    def test_lines_only(self):
>       assert parts[0] == "3"
E       AssertionError: assert '4' == '3'
```

The `>` shows which assertion failed.
The `E` shows what was wrong (expected '3', got '4').

## Common Patterns in Our Tests

### Testing stdin input:
```python
result = subprocess.run([BINARY], input="data", capture_output=True, text=True)
```

### Testing file input:
```python
result = subprocess.run([BINARY, "file.txt"], capture_output=True, text=True)
```

### Testing command-line options:
```python
result = subprocess.run([BINARY, "-l", "-w"], input="data", ...)
```

### Checking output:
```python
assert result.stdout.strip() == "expected"
assert "substring" in result.stdout
assert result.returncode == 0  # success
assert result.returncode != 0  # error expected
```

## Tips for Writing Your Own Tests

1. **Test one thing at a time** - Each test should verify one specific behavior
2. **Use descriptive names** - `test_counts_empty_file()` is better than `test1()`
3. **Add docstrings** - Explain what the test checks
4. **Test edge cases** - Empty input, very long input, special characters, etc.
5. **Test error conditions** - What happens with bad input?

## Learn More

- **Official Pytest Documentation:** https://docs.pytest.org/
- **Pytest Quick Start:** https://docs.pytest.org/en/stable/getting-started.html
- **Running Tests:** https://docs.pytest.org/en/stable/how-to/usage.html

---

**Remember:** The goal of testing is to verify your program works correctly. If tests fail, it's telling you something needs to be fixed. Don't modify the tests to make them pass - fix your code!
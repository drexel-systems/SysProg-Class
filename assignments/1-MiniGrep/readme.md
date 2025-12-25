## HW1: C Programming Refresher - minigrep

#### Description
The purpose of this assignment is a warm up to refresh your memory about how to program in C. 

You will be writing a simplified version of the `grep` utility called **minigrep**. The program searches for a pattern (string) in a text file and displays matching lines. This is a fundamental tool you'll use throughout the course when debugging, analyzing logs, and searching code.

The command line for this program is as follows:

```bash
$ minigrep -[h|n|i|c] "pattern" filename

where:
  -h    prints help about the program
  -n    prints matching lines with line numbers
  -i    performs case-insensitive search (matches Pattern, PATTERN, pattern)
  -c    counts the number of matching lines (prints count only)
  (no flag) prints all matching lines
```

#### Example Usage

```bash
$ minigrep "error" logfile.txt
Line with error in it
Another error occurred

$ minigrep -n "TODO" main.c
15: // TODO: implement error handling
42: // TODO: add validation

$ minigrep -i "warning" build.log
WARNING: deprecated function used
Warning: unused variable
This is a Warning message

$ minigrep -c "success" test_results.txt
Matches found: 23
```

#### Assignment Requirements

The purpose of this assignment is to practice / refresh your memory working with memory, pointers, file I/O, and creating functions. Your implementation must adhere to the following requirements:

1. You will not be able to use any string processing functions from the standard library (e.g., no `strcpy()`, no `strlen()`, no `strcmp()`, no `strstr()`). You will be operating on string buffers using pointers only. This also means you cannot use array notation for processing any strings, **you must operate on individual characters using pointer notation only!**

2. For the purpose of this assignment, you may use the following functions from the C standard library:
   - File I/O: `fopen()`, `fclose()`, `fgets()`
   - Output: `printf()`, `putchar()`
   - Memory: `malloc()`, `free()`
   - Character testing: `tolower()`, `toupper()` (for case-insensitive matching)
   - Program control: `exit()`

3. Since we will be composing utilities in the shell during this term, it's common that most utilities return a value. For this assignment use the `exit()` function to return a value to the shell. The values that should be used are:
   - `0` = success (pattern found)
   - `1` = pattern not found
   - `2` = command line argument error
   - `3` = file error (cannot open, read error)
   - `4` = memory allocation failure

#### What You Need to Do

Take a look at the starter code provided. It should compile and run with the provided `makefile`. You should expect some warnings initially because variables are defined but not yet used.

1. **Allocate space for the line buffer** using `malloc()`. Use the provided `line_buffer` variable as a pointer to storage. This buffer must be exactly 256 bytes. Instead of hard-coding 256, use the `#define LINE_BUFFER_SZ` provided in the starter code. Don't forget to check if `malloc()` succeeds.

2. **Implement and comment the `str_len()` function.** This function takes a pointer to a C string (null-terminated) and returns the length of the string (not including the null terminator). You must implement this yourself - do not use `strlen()` from the standard library. This function will be useful throughout your implementation.

3. **Implement and comment the `str_match()` function.** This function accepts three arguments:
   - A pointer to the line buffer (the line you're searching in)
   - A pointer to the pattern string (what you're searching for)
   - A flag indicating case-sensitive (0) or case-insensitive (1) search
   
   The function should return:
   - `1` if the pattern is found anywhere in the line
   - `0` if the pattern is not found
   
   **IMPORTANT:** For case-insensitive search, you should compare characters after converting them to the same case using `tolower()` or `toupper()`. Remember to search for the pattern anywhere in the line, not just at the beginning.

4. **Implement the file reading logic.** In `main()`, after command line parsing:
   - Open the file using `fopen()`. Check if it succeeds.
   - Read the file line by line using `fgets()` into your line buffer
   - Keep track of the current line number (starts at 1)
   - For each line, call `str_match()` to check if the pattern exists
   - Handle the output based on the selected option (see next step)
   - Don't forget to close the file when done

5. **Implement the option handlers.** The starter code has a switch statement for handling different flags:
   
   - **Default (no flag):** Print each line that contains the pattern
   
   - **`-n` (line numbers):** Print matching lines with their line numbers in the format:
     ```
     15: matching line here
     42: another matching line
     ```
   
   - **`-i` (case-insensitive):** Print matching lines, but ignore case when searching
   
   - **`-c` (count):** Count the number of matching lines and print at the end:
     ```
     Matches found: 23
     ```
     If no matches found, print:
     ```
     No matches found
     ```
   
   You may combine flags, for example `-ic` would do a case-insensitive count, and `-in` would do case-insensitive search with line numbers.

6. **Memory cleanup.** Don't forget to free your line buffer before exiting the program.

7. **Answer the questions** in `questions.md`. Do not forget to answer these questions and include them in your commit!

#### Extra Credit (+5)

Implement support for multiple files. If more than one filename is provided on the command line, search all files and print the filename before each matching line:

```bash
$ minigrep -n "error" file1.txt file2.txt file3.txt
file1.txt:10: error occurred here
file1.txt:25: another error
file2.txt:5: error in second file
```

#### Extra Credit (+10)

Implement the `-v` (invert match) option that prints all lines that do NOT contain the pattern. This is useful for filtering out unwanted lines. For example:

```bash
$ minigrep -v "debug" logfile.txt
# prints all lines that don't contain "debug"
```

You can combine this with other flags like `-n` or `-c`.

#### Extra Credit: Memory Debugging (+10)

Want to prove your code has zero memory errors? Learn to use Valgrind - the industry-standard memory debugging tool. Check out the **[Valgrind Extra Credit Assignment](valgrind-ec.md)** (10 points).

You'll use AI tools to research Valgrind independently, run it on your minigrep, fix any memory errors, and document what you learned. This is optional but highly recommended for understanding memory management at a professional level.

---

#### Grading Rubric

- 25 points: Correct implementation of required functionality
  - Basic pattern matching works correctly (5 points)
  - Line number option (-n) works (5 points)
  - Case-insensitive option (-i) works (5 points)
  - Count option (-c) works (5 points)
  - Proper error handling and memory management (5 points)
- 5 points: Code quality (readable, well-commented, good function design)
- 5 points: Answering the written questions in [questions.md](./questions.md)
- 5 points: [EXTRA CREDIT] Support for multiple files
- 10 points: [EXTRA CREDIT] Implement invert match with -v

Total points achievable is 50/35.

#### Testing Your Implementation

This assignment uses **pytest** for automated testing. Before running tests, you need to set up your Python environment.

##### Setting Up pytest (Linux)

On Linux, use a virtual environment to manage Python dependencies:

```bash
# 1. Create a virtual environment
python3 -m venv .venv

# 2. Activate the virtual environment
source .venv/bin/activate

# 3. Install pytest and dependencies
pip install -r requirements.txt

# 4. Verify pytest is working
pytest --version
```

**Note:** After activating the virtual environment, your prompt should show `(.venv)` at the beginning. You'll need to activate it each time you open a new terminal session with `source .venv/bin/activate`.

##### Running Tests

Once pytest is set up, you can run the test suite:

```bash
# Make sure your virtual environment is activated
source .venv/bin/activate

# Run all tests
pytest test_minigrep.py -v

# Or use the makefile (if configured)
make test
```

You can also test manually:

```bash
$ make
$ ./minigrep "TODO" minigrep.c
$ ./minigrep -n "include" minigrep.c
$ ./minigrep -i "ERROR" test_data.txt
$ ./minigrep -c "function" minigrep.c
```

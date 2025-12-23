# CS Systems Programming - Warmup Assignment

## Assignment Overview

This warmup assignment ensures your development environment is properly configured for the course. You will:
1. Set up a Linux environment
2. Install necessary development tools
3. Configure Git to work with your GitHub repository
4. Compile a C program that implements a word counting utility
5. Run automated tests to verify correctness
6. Submit your work via GitHub

**Points:** 5  
**Due Date:** [Instructor to specify]

---

## Part 1: Linux Environment Setup

You have two options for your Linux environment:

### Option A: Drexel's tux Cluster (Recommended for Beginners)

The tux cluster is already configured with all necessary tools. To access:

```bash
ssh username@tux.cs.drexel.edu
```

Replace `username` with your Drexel username. All development tools are pre-installed.

**If using tux, skip to Part 3.**

### Option B: Self-Hosted Linux VM

If you prefer to work on your personal computer, you can set up a local Linux environment.  Please note however if you work on a personal virtual machine you will be repsonsible for all aspects of installing and supporting this virtual machine. 

**For macOS users:**
- Install [OrbStack](https://orbstack.dev/) (recommended) or another Virtual Machine hypervisor for MacOS.  Some ones that I have used in the past are VMWare Fusion, Oracle VirtualBox, or Parallels (commercial).  Note that OrbStack is reccomended for its simplicity, and it is what I will be using in this class.  While OrbStack is commercial software, you are allowed to use it for free for non-commercial work such as to support coursework. 
- Create an Ubuntu 24.04 LTS container/VM

**For Windows users:**
- Install [Windows Subsystem for Linux (WSL2)](https://learn.microsoft.com/en-us/windows/wsl/install)
- Install Ubuntu 24.04 LTS from the Microsoft Store

**For Linux users:**
- You're already set! Just ensure you're on a recent distribution.

**Important:** All course materials and demos are tested on **Ubuntu 24.04 LTS**. While other distributions should work, Ubuntu 24.04 LTS is strongly recommended to avoid compatibility issues.

---

## Part 2: Development Tools Installation

**Note:** Skip this section if you're using the tux cluster.

Once you have your Linux environment running, install the development tools:

```bash
sudo apt update && sudo apt install -y \
  build-essential \
  gcc \
  clang \
  gdb \
  llvm \
  lldb \
  python3 \
  python3-pip \
  valgrind \
  binutils \
  git \
  vim
```

**What these tools do:**
- `build-essential`: Core compilation tools (including `make`)
- `gcc` & `clang`: C compilers
- `gdb` & `lldb`: Debuggers
- `valgrind`: Memory leak detector
- `python3` & `python3-pip`: For running test harnesses
- `git`: Version control
- `vim`: Text editor (feel free to use your preferred editor)

**Verification:** After installation completes, verify the tools are available:

```bash
gcc --version
make --version
python3 --version
```

Each command should print version information without errors.

---

## Part 3: Git and GitHub Configuration

You have been provided with a GitHub repository for this course. All assignments will be completed in subdirectories of this repository.

### Step 1: Set Up Your SSH Key

You have been provided with an SSH keypair to access your GitHub repository. Place the private key in your `.ssh` directory:

```bash
# Create .ssh directory if it doesn't exist
mkdir -p ~/.ssh

# Copy your provided private key to ~/.ssh/
# Replace 'id_rsa_drexel' with the actual filename provided
cp /path/to/your/provided/key ~/.ssh/id_rsa_drexel

# Set proper permissions (critical for SSH to work)
chmod 600 ~/.ssh/id_rsa_drexel
```

### Step 2: Configure Git

Set up your Git identity (use your Drexel email):

```bash
git config --global user.name "Your Name"
git config --global user.email "yourname@drexel.edu"
```

Configure Git to use your SSH key:

```bash
# Create or edit ~/.ssh/config
cat >> ~/.ssh/config << EOF
Host github.com
    HostName github.com
    User git
    IdentityFile ~/.ssh/id_rsa_drexel
EOF

chmod 600 ~/.ssh/config
```

### Step 3: Clone Your Repository

Clone the repository provided by your instructor:

```bash
# Replace with your actual repository URL
git clone git@github.com:drexel-cs/your-repo-name.git
cd your-repo-name
```

### Step 4: Create the Assignment Directory

```bash
# Create the directory for this warmup assignment
mkdir -p 00-Warmup
cd 00-Warmup
```

You will complete all work for this assignment in the `00-Warmup` directory. Future assignments will have their own directories (e.g., `01-Assignment`, `02-Assignment`, etc.).

### Git Workflow Reminder

As you work on the assignment:

1. **Commit frequently** with meaningful messages:
   ```bash
   git add wordcount.c
   git commit -m "Implement basic word counting logic"
   ```

2. **Push to GitHub regularly** to backup your work:
   ```bash
   git push origin main
   ```

3. **Good commit messages** describe what changed and why:
   - Good: "Fix word counting to handle multiple spaces"
   - Bad: "updates" or "fixed bug"

**Note:** Your repository should include a `.gitignore` file that excludes temporary files like `.venv/`, compiled binaries (`wordcount`), and editor temp files. If it doesn't, create one in your repository root:

```bash
# From your repository root
cd ~/your-repo-name
cat > .gitignore << EOF
.venv/
*.o
wordcount
*.pyc
__pycache__/
.DS_Store
*~
EOF
git add .gitignore
git commit -m "Add .gitignore for temp files"
git push origin main
```

---

## Part 4: The Programming Assignment

### Assignment Description

You will work with an instructor provided version of a tool similiar to the Unix `wc` (word count) utility called `wordcount`. This program will count lines, words, and characters in input from either standard input (stdin) or files.

### Requirements

Your program basically demonstrates:

1. **Accept command-line options:**
   - `-l`: Count lines only
   - `-w`: Count words only  
   - `-c`: Count characters only
   - If no options are given, count all three (lines, words, characters)

2. **Accept file arguments:**
   - If no files are specified, read from standard input
   - If one or more files are specified, process each file
   - If multiple files are given, print individual counts for each file plus a total

3. **Handle errors gracefully:**
   - Print an error message if a file cannot be opened
   - Return exit code 0 on success, non-zero on error

### Example Usage

```bash
# Count all metrics from stdin
echo "Hello world" | ./wordcount
       1       2      12

# Count lines only from a file
./wordcount -l myfile.txt
       5 myfile.txt

# Count lines and words from multiple files
./wordcount -l -w file1.txt file2.txt
      10      50 file1.txt
       5      25 file2.txt
      15      75 total
```

---

## Part 5: Getting Started

### Step 1: Download the Assignment Files

Download or copy these files to your `00-Warmup` directory in your cloned repository:
- `wordcount.c` - Your C source code (provided as a starter)
- `Makefile` - Build configuration (provided)
- `test_wordcount.py` - Automated tests (provided)

```bash
cd ~/your-repo-name/00-Warmup
# Copy or download the provided files here
```

### Step 2: Understanding the Makefile

The provided `Makefile` automates compilation. Open it and examine the contents:

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11
TARGET = wordcount

all: $(TARGET)

$(TARGET): wordcount.c
	$(CC) $(CFLAGS) -o $(TARGET) wordcount.c

clean:
	rm -f $(TARGET)

.PHONY: all clean
```

**Key points:**
- `CC`: Specifies the compiler (gcc)
- `CFLAGS`: Compiler flags
  - `-Wall -Wextra`: Enable all warnings
  - `-g`: Include debugging symbols (needed for gdb)
  - `-std=c11`: Use C11 standard
- `TARGET`: Name of the output binary
- The recipe compiles `wordcount.c` directly to `wordcount` (no intermediate `.o` files)
- `clean`: Removes the compiled binary

### Step 3: Compile Your Program

```bash
make
```

This should produce a `wordcount` executable in your current directory. If there are compilation errors, fix them before proceeding.

**To recompile after changes:**
```bash
make clean
make
```

### Step 4: Manual Testing

Before running automated tests, try your program manually:

```bash
# Test with stdin
echo "Hello world" | ./wordcount

# Test with a file
echo -e "Line 1\nLine 2\nLine 3" > test.txt
./wordcount test.txt

# Test with options
./wordcount -l test.txt
./wordcount -w test.txt
./wordcount -l -w test.txt

# Test with multiple files
echo "File A" > a.txt
echo "File B" > b.txt
./wordcount a.txt b.txt
```

---

## Part 6: Setting Up pytest

### Step 1: Create a Python Virtual Environment

A virtual environment isolates Python packages for this project:

```bash
# Navigate to your assignment directory
cd ~/your-repo-name/00-Warmup

# Create a virtual environment
python3 -m venv .venv

# Activate the virtual environment
source .venv/bin/activate
```

Your prompt should now show `(.venv)` indicating the virtual environment is active.

**Note:** The `.venv` directory should NOT be committed to Git. It's local to your machine.

**To deactivate later:** Type `deactivate` (but don't do this yet).

### Step 2: Install pytest

With the virtual environment activated:

```bash
pip install pytest
```

Verify installation:
```bash
pytest --version
```

### Step 3: Run the Tests

```bash
pytest test_wordcount.py -v
```

The `-v` flag provides verbose output showing each test.

**Expected output:**
```
test_wordcount.py::TestBasicFunctionality::test_stdin_all_counts PASSED
test_wordcount.py::TestBasicFunctionality::test_file_all_counts PASSED
test_wordcount.py::TestOptions::test_lines_only PASSED
...
======================== X passed in Y.YYs ========================
```

### Understanding Test Results

- **PASSED**: Test succeeded ✓
- **FAILED**: Test failed - your program's output didn't match expectations
- **ERROR**: Test couldn't run (usually compilation or runtime error)

If tests fail, read the error messages carefully. They show:
- Which test failed
- What output was expected
- What output your program produced

---

## Part 7: Debugging Failed Tests

If tests fail:

1. **Read the error message carefully** - pytest shows expected vs actual output

2. **Run the specific test case manually:**
   ```bash
   # For example, if test_lines_only failed:
   echo -e "Line 1\nLine 2\nLine 3" | ./wordcount -l
   ```

3. **Use gdb for debugging:**
   ```bash
   gdb ./wordcount
   (gdb) run -l < test.txt
   (gdb) break main
   (gdb) next
   ```

4. **Check for common issues:**
   - Off-by-one errors in counting
   - Not handling edge cases (empty input, no trailing newline)
   - Incorrect word boundary detection
   - Format of output (spacing, column width)

---

## Submission Requirements

### What to Submit

Your GitHub repository's `00-Warmup` directory should contain:

1. **wordcount.c** - Your source code
2. **Makefile** - The build configuration  
3. **test_wordcount.py** - The pytest test file
4. **Warmup-Solution.png** or **Warmup-Solution.jpg** - Screenshot of your pytest output

### Creating the Screenshot

After all tests pass, capture the pytest output:

```bash
pytest test_wordcount.py -v
```

Take a screenshot of your terminal showing the pytest results. Save it as either `Warmup-Solution.png` or `Warmup-Solution.jpg` in your `00-Warmup` directory.

On most Linux systems:
- Use screenshot tools built into your environment
- Or use `gnome-screenshot`, `scrot`, or similar tools
- Make sure the entire test output is visible

### Pushing to GitHub

Before submitting, ensure all your work is pushed to GitHub:

```bash
cd ~/your-repo-name/00-Warmup
git add wordcount.c Makefile test_wordcount.py Warmup-Solution.png
git commit -m "Complete warmup assignment"
git push origin main
```

Verify your files are visible on GitHub by visiting your repository in a web browser.

### Submitting on Canvas

1. Go to Canvas and find the Warmup Assignment
2. Submit the **URL to your GitHub repository**
3. Example: `https://github.com/drexel-cs/your-repo-name`

That's it! We will clone your repository and grade the contents of your `00-Warmup` directory.

---

## Grading

This assignment is worth **5 points** and is graded as follows:

- **5 points**: All pytest tests pass, screenshot provided, code submitted to GitHub
- **0 points**: Tests fail or submission incomplete

This is a pass/fail assignment designed to verify your development environment is working correctly.

---

## Additional Resources

### Useful Commands

```bash
# Check for memory leaks
valgrind --leak-check=full ./wordcount test.txt

# Compile with different compilers
clang -Wall -Wextra -g -o wordcount wordcount.c

# View file contents
cat wordcount.c
less wordcount.c
```

### Testing Your Environment

Create a simple test program to verify your setup:

```c
// test.c
#include <stdio.h>
int main() {
    printf("Environment is working!\n");
    return 0;
}
```

```bash
gcc -o test test.c
./test
```

### Getting Help

If you encounter issues, check the course Canvas page for office hours and additional support resources.

---

## Learning Objectives

By completing this assignment, you will:

- Set up a professional C development environment
- Understand the compilation process and Makefiles
- Work with command-line arguments in C
- Practice file I/O and stream processing
- Use automated testing to verify correctness
- Gain experience with standard Unix development tools

Good luck, and welcome to Systems Programming!
# System Call Analysis with strace

**Assignment Component:** Required (10 points)  
**Difficulty:** Intermediate - Requires Independent Research  
**Skills:** System Call Tracing, File I/O Analysis, Self-Directed Learning

---

## The Challenge

You've implemented a database using Linux system calls (open, read, write, lseek, close). But how do you **prove** your implementation is correct? How do you see what's actually happening at the operating system level?

**Your task:** Use `strace` to trace and analyze the system calls your database makes. Verify your implementation is correct, understand sparse file behavior, and document what you discover.

**Specifically, you need to:**
1. Learn how to use `strace` for system call analysis
2. Trace your database operations (add, delete, read students)
3. Analyze the system calls and their parameters
4. Investigate sparse file creation and behavior
5. Document your findings and learning process using AI tools

**The approach:** Use AI tools (ChatGPT, Claude, Gemini, etc.) to research `strace` independently. This is a required component, not extra credit.

---

## Why This Matters

**In systems programming:**
- Your code might compile and seem to work, but are the system calls correct?
- `strace` lets you see the actual system calls your program makes
- It's the definitive way to debug system-level behavior
- Essential for understanding performance and correctness

**Professional reality:**
- Every systems programmer uses `strace` for debugging
- It's the standard tool for tracing system calls
- You'll use it throughout your career for systems troubleshooting
- Understanding system call behavior is crucial for performance optimization

**For this assignment:**
- Validates your lseek() offsets are correct
- Shows how sparse files are created
- Proves your read/write operations work correctly
- Helps debug if operations aren't working as expected

---

## Getting Started: Key Questions to Explore

Use AI tools to research and discover answers to these questions:

### Understanding Phase

1. **What is strace?** What does it do and why is it used?

2. **How do you install strace?** On your Linux environment (tux or VM)?

3. **What does strace show you?** What information is in the output?

4. **How do you run strace on a program?** What's the basic syntax?

### Basic Tracing Phase

5. **How do you trace a program with arguments?** Your `sdbsc` program needs arguments like `-a 1 john doe 350`

6. **What does the strace output format mean?** How do you read a line like:
   ```
   open("student.db", O_RDWR|O_CREAT, 0666) = 3
   ```

7. **How do you filter strace output?** You only care about file operations, not all system calls.

8. **What system calls should you see?** For your database: open, lseek, read, write, close

### Analysis Phase

9. **How do you see system call parameters?** Can you see the file descriptor, offset, buffer size?

10. **What does lseek() look like in strace?** How can you verify the offset calculation?

11. **How can you tell if a hole was created?** What does strace show when lseek() skips ahead?

12. **How do you save strace output?** You'll need it for your analysis document.

---

## Learning Strategy: Using AI Effectively

### Research Approach

1. **Start broad**: "What is strace and how does it work?" → "How do I use strace?"
2. **Get specific**: Tell the AI about your database program and what syscalls you're using
3. **Share your output**: Paste strace output and ask AI to help interpret it
4. **Iterate**: Try different strace options and ask AI what they do
5. **Validate**: Compare what strace shows with what your code does

### When You Get Stuck

- Share your strace output with AI (paste relevant lines)
- Ask about specific system call parameters you don't understand
- Request help filtering or formatting the output
- Compare different operations to see patterns

### Critical Thinking

**Remember:**
- strace shows the actual system calls - it's ground truth
- If strace shows different offsets than your code calculates, strace is right
- Every lseek, read, and write will appear in the trace
- System calls that fail show error codes

---

## What You Need to Deliver

### File: `strace-analysis.md`

Create this file in your assignment directory with the following sections:

### 1. Learning Process (2 points)

Document how you learned strace:
- What AI tools did you use?
- What questions did you ask? (Include 3-4 specific prompts)
- What resources did the AI point you to?
- What challenges did you encounter learning strace?

**Example:**
```
I used ChatGPT to learn strace. I started by asking "What is strace and 
how do I use it to trace system calls in C programs?" Then I asked "How 
do I run strace on a program with command line arguments?" When I got 
too much output, I asked "How do I filter strace to show only file 
operations like open, read, write, lseek, and close?"
```

### 2. Basic System Call Analysis (3 points)

Analyze three operations. For each, provide strace output and analysis:

#### A. Adding a Student

Run: `strace -e trace=open,lseek,read,write,close ./sdbsc -a 1 john doe 350`

**Provide:**
- The strace output (you can trim to relevant syscalls)
- Identify each system call and explain what it does
- Verify the lseek offset is correct (should be 1 * 64 = 64)
- Verify the write() size is correct (should be 64 bytes)

**Example analysis:**
```
System call sequence for adding student ID=1:

1. open("student.db", O_RDWR|O_CREAT, 0666) = 3
   - Opens database file for reading/writing, creates if needed
   - Returns file descriptor 3

2. lseek(3, 64, SEEK_SET) = 64
   - Seeks to byte 64 (student ID 1 * 64 bytes)
   - This is correct for student ID=1
   - Creates a "hole" from bytes 0-63

3. write(3, "...", 64) = 64
   - Writes 64-byte student_t structure
   - Returns 64 (all bytes written successfully)

4. close(3) = 0
   - Closes the database file
```

#### B. Reading/Printing a Student

Run: `strace -e trace=open,lseek,read,write,close ./sdbsc -g 1`

**Provide:**
- The strace output
- Identify the lseek to find the student
- Identify the read to get student data
- Verify the offset and size are correct

#### C. Deleting a Student

Run: `strace -e trace=open,lseek,read,write,close ./sdbsc -d 1`

**Provide:**
- The strace output
- Note: deletion writes zeros - look for write() call
- Identify if there's a read before the write (checking if student exists)
- Verify the lseek offset and write size

### 3. Sparse File Investigation (3 points)

This is the most interesting part! Investigate how sparse files work.

#### A. Create a Fresh Database

```bash
rm student.db  # Start fresh
strace -e trace=open,lseek,write,close ./sdbsc -a 1 john doe 350
ls -lh student.db
du -h student.db
```

**Answer these questions:**

1. **What is the file size reported by `ls -lh`?**
   - Should be 128 bytes (2 * 64)
   - Explain why

2. **What is the actual disk usage reported by `du -h`?**
   - Should be 4K
   - Explain why it's larger than 128 bytes but not as much as it could be

3. **In the strace output, what did lseek() do?**
   - It skipped from byte 0 to byte 64
   - This creates a "hole" in the file (bytes 0-63)
   - Holes don't take up disk space

#### B. Add a Student with Large ID

```bash
strace -e trace=lseek,write ./sdbsc -a 99999 big id 400
ls -lh student.db
du -h student.db
```

**Answer these questions:**

1. **What offset did lseek() seek to?**
   - Calculate: 99999 * 64 = ?
   - Does strace show this offset?

2. **What is the file size now?**
   - Should be huge (6.4 MB)
   - But du shows actual usage is still small

3. **What happened?**
   - lseek created a HUGE hole
   - Only 2 student records actually written (student 1 and 99999)
   - Sparse file only allocates space for written data

#### C. Sparse File Explanation

Based on your investigation, explain:
- What is a sparse file?
- How does lseek() create holes?
- Why is this efficient for our database?
- What would happen without sparse file support?

### 4. System Call Verification (2 points)

Verify your implementation is correct by checking:

**Checklist:**
- [ ] open() opens the database file with correct flags
- [ ] lseek() offsets match the formula: `id * 64`
- [ ] write() always writes exactly 64 bytes
- [ ] read() reads exactly 64 bytes when getting a student
- [ ] close() is called to close the file
- [ ] No errors (return values are non-negative)

**Questions to answer:**
1. Did you find any bugs in your implementation through strace analysis?
2. Do all your system calls return success (non-negative values)?
3. Are your lseek() offsets calculated correctly?
4. Do you read/write the correct number of bytes?

If you found bugs, describe what was wrong and how you fixed it.

---

## Technical Requirements

### strace Commands to Use

**Basic tracing:**
```bash
strace -e trace=open,lseek,read,write,close ./sdbsc -a 1 john doe 350
```

**Save output to file:**
```bash
strace -e trace=open,lseek,read,write,close -o trace.txt ./sdbsc -a 1 john doe 350
```

**Follow all syscalls (noisy but comprehensive):**
```bash
strace ./sdbsc -a 1 john doe 350 2>&1 | less
```

### Including strace Output in Your Document

Use code blocks with clear labels:

```
Operation: Adding student ID=1
Command: strace -e trace=open,lseek,write,close ./sdbsc -a 1 john doe 350

Output:
open("student.db", O_RDWR|O_CREAT, 0666) = 3
lseek(3, 64, SEEK_SET)                  = 64
write(3, "\1\0\0\0john\0\0\0\0\0\0\0\0..."..., 64) = 64
close(3)                                = 0
```

---

## Grading Rubric

**10 points total:**

**Learning Process (2 points)**
- 2 pts: Clear documentation of AI-assisted learning with specific examples
- 1 pt: Vague description of learning process
- 0 pts: No evidence of learning process

**Basic System Call Analysis (3 points)**
- 3 pts: All three operations traced and analyzed correctly
- 2 pts: Two operations analyzed well
- 1 pt: One operation analyzed
- 0 pts: No meaningful analysis

**Sparse File Investigation (3 points)**
- 3 pts: Thorough investigation with correct explanations
- 2 pts: Good investigation, minor gaps in understanding
- 1 pt: Basic investigation, significant gaps
- 0 pts: No investigation or incorrect

**System Call Verification (2 points)**
- 2 pts: Thorough verification, identifies any bugs found
- 1 pt: Basic verification, incomplete
- 0 pts: No verification or incorrect

---

## Hints for Success

### Running strace

**Your program needs arguments:**
```bash
strace ./sdbsc -a 1 john doe 350        # Correct
strace ./sdbsc                          # Won't work - needs args
```

**Filter to relevant syscalls:**
```bash
strace -e trace=open,lseek,read,write,close ./sdbsc -a 1 john doe 350
```

**Save output (strace writes to stderr):**
```bash
strace -e trace=open,lseek,read,write,close ./sdbsc -a 1 john doe 350 2> trace.txt
```

### Understanding System Call Output

**System call format:**
```
syscall_name(arg1, arg2, ...) = return_value
```

**Example:**
```
lseek(3, 64, SEEK_SET) = 64
```
- Function: lseek
- Args: fd=3, offset=64, whence=SEEK_SET
- Return: 64 (new position in file)

### Calculating Offsets

For student ID `n`:
- Offset = `n * 64`
- Student ID 1: offset = 64
- Student ID 100: offset = 6400
- Student ID 99999: offset = 6399936

### Understanding Sparse Files

**Key concepts:**
- Logical file size: what `ls` reports
- Physical disk usage: what `du` reports
- Hole: gap in file created by lseek, contains zeros, uses no disk space
- Block size: typically 4096 bytes (4K)

**Why sizes differ:**
- `ls` shows logical size (includes holes)
- `du` shows actual disk usage (excludes holes)
- File system allocates in blocks (4K chunks)

---

## Common Issues

**strace output is too verbose:**
- Use `-e trace=open,lseek,read,write,close` to filter
- Or use `grep` to filter output: `strace ./sdbsc -a 1 john doe 350 2>&1 | grep lseek`

**Can't find student.db operations:**
- Make sure database file is being opened
- Check for errors in open() call
- Verify your program arguments are correct

**lseek offsets look wrong:**
- Remember: offset = id * 64
- Check your calculation in your code
- Compare with what strace shows

**File sizes don't match expectations:**
- Remember sparse files: ls shows logical size
- Use `du` to see actual disk usage
- Empty space (holes) doesn't use disk space

---

## Resources

- `man strace` - comprehensive strace documentation
- `man 2 open`, `man 2 lseek`, `man 2 read`, `man 2 write` - syscall documentation
- Your AI tool of choice (ChatGPT, Claude, Gemini, etc.)
- Online resources about sparse files

---

## Example: What Good Analysis Looks Like

Here's what a strong system call analysis might include:

### Adding Student ID=50

**Command:**
```bash
strace -e trace=open,lseek,read,write,close ./sdbsc -a 50 jane doe 385
```

**Output:**
```
open("student.db", O_RDWR|O_CREAT, 0666) = 3
lseek(3, 3200, SEEK_SET)                = 3200
write(3, "\62\0\0\0jane\0\0\0\0\0\0\0\0..."..., 64) = 64
close(3)                                = 0
```

**Analysis:**

**1. File Opening:**
```
open("student.db", O_RDWR|O_CREAT, 0666) = 3
```
- Opens database file for read/write (`O_RDWR`)
- Creates file if it doesn't exist (`O_CREAT`)
- Sets permissions to 0666 (rw-rw-rw-)
- Returns file descriptor 3 (0,1,2 are stdin/stdout/stderr)

**2. Seeking to Position:**
```
lseek(3, 3200, SEEK_SET) = 3200
```
- Seeks to byte 3200 in file
- Calculation: 50 * 64 = 3200 ✓ CORRECT
- SEEK_SET means absolute position from start of file
- Return value 3200 confirms new position

**3. Writing Student Record:**
```
write(3, "...", 64) = 64
```
- Writes to file descriptor 3 (our database)
- Writes exactly 64 bytes (size of student_t) ✓ CORRECT
- Return value 64 means all bytes written successfully
- The buffer contains the student_t structure with ID=50

**4. Closing File:**
```
close(3) = 0
```
- Closes the database file
- Return value 0 indicates success
- File descriptor 3 is now invalid

**Verification:**
- ✓ Offset calculation correct (50 * 64 = 3200)
- ✓ Write size correct (64 bytes)
- ✓ All system calls succeeded (non-negative returns)
- ✓ File properly opened and closed

---

## Final Thought

strace shows you **ground truth** - the actual system calls your program makes at the OS level. Your code might claim to seek to a certain position, but strace proves what actually happened. This is invaluable for:
- Verifying your implementation is correct
- Debugging system call issues
- Understanding how the OS manages files
- Learning how sparse files work in practice

The goal isn't just to trace some system calls - it's to **understand your database at the system call level** and verify it works exactly as specified.

**Good luck with your analysis!**
# System Call Analysis with strace: Pipes and File Descriptors

**Assignment Component:** Required (10 points)  
**Difficulty:** Advanced - Deep System Understanding  
**Skills:** Pipe Tracing, File Descriptor Manipulation, Self-Directed Learning

---

## The Challenge

You've implemented pipes to connect commands together. But how do you **verify** your implementation is correct? How do you see the actual pipe() and dup2() system calls? How do you ensure you're not leaking file descriptors?

**Your task:** Use `strace` to trace and analyze pipe operations in your shell. Understand how pipes are created, how file descriptors are manipulated with dup2(), and why closing unused pipes is critical.

**Specifically, you need to:**
1. Learn how to use `strace` for pipe and file descriptor tracing
2. Trace your shell's pipe() and dup2() operations
3. Analyze file descriptor lifecycle (create, dup, close)
4. Investigate why closing unused pipes is critical
5. Document your findings and learning process using AI tools

**The approach:** Use AI tools (ChatGPT, Claude, Gemini, etc.) to research `strace` for pipe analysis. This is a required component, not extra credit.

---

## Why This Matters

**In systems programming:**
- Pipes are invisible - you can't see data flowing
- File descriptor leaks cause processes to hang
- strace reveals the actual pipe plumbing
- Essential for debugging pipeline issues

**Professional reality:**
- Pipes are fundamental to Unix/Linux
- strace is THE tool for understanding pipes
- File descriptor management is critical
- Common source of production bugs

**For this assignment:**
- Validates your pipe() calls create correct fds
- Shows dup2() redirecting stdin/stdout
- Proves you close all unused pipe ends
- Helps debug hanging processes

---

## Getting Started: Key Questions to Explore

Use AI tools to research and discover answers to these questions:

### Understanding Phase

1. **What is a pipe at the system call level?** How does pipe() work?

2. **What are file descriptors?** What numbers do pipes use?

3. **How do you trace pipe operations with strace?** What flags to use?

4. **What does dup2() do?** How does it redirect stdin/stdout?

### Pipe Tracing Phase

5. **How do you see pipe() in strace output?** What does it return?

6. **What file descriptor numbers do pipes get?** Start with 3, 4, 5...?

7. **How can you trace both pipe() and dup2()?** Filter strace output?

8. **How do you trace all children?** The -f flag is critical!

### File Descriptor Analysis Phase

9. **How many file descriptors does a two-command pipe create?**

10. **What happens when you dup2(pipefd[0], STDIN_FILENO)?**

11. **Why must you close() pipes after dup2()?**

12. **What happens if you DON'T close unused pipe ends?** Try it!

---

## Learning Strategy: Using AI Effectively

### Research Approach

1. **Start with pipes**: "How do pipe() and dup2() system calls work?"
2. **Get specific**: "How do I trace pipe operations with strace?"
3. **Experiment**: Run your shell with strace, share output with AI
4. **Debug**: "Why is my process hanging?" → trace with strace
5. **Understand**: Ask AI to explain file descriptor numbers

### When You Get Stuck

- Share your strace output with AI (paste relevant lines)
- Ask about specific file descriptor numbers
- Request help understanding which process has which pipes
- Compare working vs broken implementations

### Critical Thinking

**Remember:**
- pipe() creates a pair of file descriptors
- dup2() duplicates a file descriptor
- File descriptors are per-process
- Unused pipe ends MUST be closed
- Parent and children both have copies of pipes

---

## What You Need to Deliver

### File: `strace-pipe-analysis.md`

Create this file in your assignment directory with the following sections:

### 1. Learning Process (2 points)

Document how you learned strace for pipe tracing:
- What AI tools did you use?
- What questions did you ask? (Include 3-4 specific prompts)
- What resources did the AI point you to?
- What challenges did you encounter with pipe tracing?

**Example:**
```
I used ChatGPT to learn strace for pipes. I started by asking 
"How do I use strace to see pipe() and dup2() system calls?" 
Then I asked "What file descriptor numbers do pipes use?" When 
I saw my process hanging, I asked "Why would a process hang 
waiting for input from a pipe?" The AI explained that if you 
don't close all write ends, the reader never gets EOF!
```

### 2. Basic Pipe Analysis (3 points)

Trace a simple two-command pipeline. For each part, provide strace output and analysis:

#### A. Two-Command Pipe: `ls | cat`

Run your shell with strace:
```bash
strace -f -e trace=pipe,dup2,close,fork,execve ./dsh
dsh3> ls | cat
dsh3> exit
```

**Provide:**
- The relevant strace output (pipe, dup2, close calls)
- Identify the pipe() call - what fds does it create?
- Identify dup2() calls in each child
- Identify close() calls - which pipes are closed where?
- Verify both children are created

**Example analysis:**
```
Two-command pipeline: ls | cat

Parent Process:
1. pipe([3, 4]) = 0
   - Creates pipe with read end fd=3, write end fd=4
   
2. fork() = 1001
   - Creates first child (ls)
   
3. fork() = 1002
   - Creates second child (cat)

Child 1 (PID 1001) - ls command:
4. dup2(4, 1) = 1
   - Redirects stdout to pipe write end
   - stdout now writes to fd=4 (pipe)
   
5. close(3) = 0
   - Closes unused read end
   
6. close(4) = 0
   - Closes original write end (dup still exists)
   
7. execve("/bin/ls", ["ls"], ...) = 0
   - Runs ls, output goes to pipe

Child 2 (PID 1002) - cat command:
8. dup2(3, 0) = 0
   - Redirects stdin from pipe read end
   - stdin now reads from fd=3 (pipe)
   
9. close(3) = 0
   - Closes original read end (dup still exists)
   
10. close(4) = 0
    - Closes unused write end
    
11. execve("/bin/cat", ["cat"], ...) = 0
    - Runs cat, input comes from pipe

Parent Process:
12. close(3) = 0
    - Parent closes read end
    
13. close(4) = 0
    - Parent closes write end
    
Data flow: ls writes to fd=4 → cat reads from fd=3
```

#### B. Three-Command Pipe: `ls | grep txt | wc -l`

**Provide:**
- How many pipe() calls? (Should be 2 for 3 commands)
- What file descriptor numbers are created?
- How does the middle command (grep) handle both stdin and stdout?
- Verify all three children are created

#### C. File Descriptor Leak Demo

**Experiment:** Temporarily comment out ALL close() calls in your code, then trace:

```bash
strace -f -e trace=pipe,dup2,close ./dsh
dsh3> ls | cat
[process hangs?]
```

**Provide:**
- What happened? Did it hang?
- Why did it hang (or not)?
- What does strace show about open file descriptors?

### 3. File Descriptor Management (3 points)

Analyze file descriptor lifecycle for a pipeline.

**Answer these questions:**

1. **When are pipes created?**
   - Before or after fork()?
   - How many pipes for N commands?

2. **What file descriptors do pipes use?**
   - What numbers are assigned?
   - Why do they start at 3? (0=stdin, 1=stdout, 2=stderr)

3. **How does dup2() work?**
   - What does dup2(4, 1) do?
   - Why do we close fd=4 after dup2(4, 1)?

4. **Which pipes does each process close?**
   - First command in pipeline?
   - Middle command?
   - Last command?
   - Parent process?

5. **What happens if you forget to close a pipe?**
   - Which process hangs?
   - Why does it hang?
   - How does strace show this?

**Example answer for question 3:**
```
dup2(4, 1) duplicates file descriptor 4 to file descriptor 1 (stdout).
After this call:
- Both fd=1 and fd=4 point to the same pipe write end
- stdout now writes to the pipe
- We can close fd=4 because fd=1 still points to the pipe
- If we don't close fd=4, we waste a file descriptor
```

### 4. Pipeline Verification (2 points)

Use strace to verify your implementation is correct:

**Checklist:**
- [ ] pipe() called N-1 times for N commands
- [ ] Each child calls dup2() appropriately
- [ ] All children close ALL pipe file descriptors
- [ ] Parent closes all pipes after forking
- [ ] All children call execve()
- [ ] Parent waits for all children

**Questions to answer:**
1. Does your implementation create the correct number of pipes?
2. Does each child redirect stdin/stdout correctly?
3. Does each process close all unused pipe ends?
4. Are there any file descriptor leaks?

If you found bugs, describe what was wrong and how you fixed it.

---

## Technical Requirements

### strace Commands to Use

**Basic pipe tracing:**
```bash
strace -f -e trace=pipe,dup2,close ./dsh
```

**Full pipeline tracing:**
```bash
strace -f -e trace=pipe,dup2,close,fork,execve,wait4 ./dsh
```

**Save output to file:**
```bash
strace -f -e trace=pipe,dup2,close -o trace.txt ./dsh
```

**See file descriptor numbers:**
```bash
strace -f -v ./dsh 2>&1 | grep -E "(pipe|dup2|close)"
```

### Key strace Flags

- `-f` : Follow child processes (ESSENTIAL!)
- `-e trace=pipe,dup2,close` : Only show these syscalls
- `-v` : Verbose mode (shows more details)
- `-o FILE` : Save output to file
- `2>&1` : Redirect stderr to stdout

### Understanding strace Output

**pipe() creates fd pair:**
```
pipe([3, 4]) = 0
```
- Creates pipe with read end fd=3, write end fd=4
- Returns 0 on success

**dup2() redirects:**
```
dup2(4, 1) = 1
```
- Duplicates fd=4 to fd=1 (stdout)
- Returns new fd (1)

**close() closes fd:**
```
close(3) = 0
```
- Closes file descriptor 3
- Returns 0 on success

**With PIDs:**
```
[pid 1001] pipe([3, 4]) = 0
[pid 1002] dup2(4, 1) = 1
```
- Shows which process makes each call

---

## Grading Rubric

**10 points total:**

**Learning Process (2 points)**
- 2 pts: Clear documentation of AI-assisted learning with specific examples
- 1 pt: Vague description of learning process
- 0 pts: No evidence of learning process

**Basic Pipe Analysis (3 points)**
- 3 pts: All three scenarios analyzed correctly with real strace output
- 2 pts: Two scenarios analyzed well
- 1 pt: One scenario analyzed
- 0 pts: No meaningful analysis

**File Descriptor Management (3 points)**
- 3 pts: Thorough answers to all questions with correct understanding
- 2 pts: Good answers, minor gaps
- 1 pt: Basic understanding, significant gaps
- 0 pts: No understanding or incorrect

**Pipeline Verification (2 points)**
- 2 pts: Thorough verification, identifies implementation correctness
- 1 pt: Basic verification, incomplete
- 0 pts: No verification

---

## Hints for Success

### Running strace on Pipelines

**Must use -f flag:**
```bash
strace -f ./dsh        # Correct - traces all children
strace ./dsh           # Wrong - only traces parent
```

**Test with simple pipe:**
```bash
strace -f -e trace=pipe,dup2,close ./dsh
dsh3> ls | cat
dsh3> exit
```

### Understanding File Descriptors

**Standard file descriptors:**
- 0 = stdin
- 1 = stdout
- 2 = stderr
- 3+ = your pipes and files

**After pipe([3, 4]):**
- fd=3 is read end
- fd=4 is write end

**After dup2(4, 1):**
- fd=1 now points to pipe write end
- fd=4 still points to pipe write end
- Can close fd=4 (fd=1 still works)

### Finding Pipe Operations

**Search for specific syscalls:**
```bash
strace -f ./dsh 2>&1 | grep pipe
strace -f ./dsh 2>&1 | grep dup2
strace -f ./dsh 2>&1 | grep close
```

### Common Issues

**Process hangs:**
- Forgot to close write end
- Reader waits for EOF that never comes
- strace shows process blocked in read()

**Too many open files:**
- Not closing pipes
- File descriptor leak
- strace shows close() never called

**Wrong output:**
- dup2() to wrong fd
- Pipes connected incorrectly
- Check dup2() calls in strace

---

## Example: What Good Analysis Looks Like

Here's what a strong pipe analysis might include:

### Two-Command Pipeline: echo hello | cat

**Command:**
```bash
strace -f -e trace=pipe,dup2,close,fork,wait4 ./dsh
dsh3> echo hello | cat
dsh3> exit
```

**Relevant strace output:**
```
[pid 5000] pipe([3, 4])              = 0
[pid 5000] fork()                    = 5001
[pid 5001] dup2(4, 1)                = 1
[pid 5001] close(3)                  = 0
[pid 5001] close(4)                  = 0
[pid 5000] fork()                    = 5002
[pid 5002] dup2(3, 0)                = 0
[pid 5002] close(3)                  = 0
[pid 5002] close(4)                  = 0
[pid 5000] close(3)                  = 0
[pid 5000] close(4)                  = 0
[pid 5000] wait4(-1, NULL, 0, NULL)  = 5001
[pid 5000] wait4(-1, NULL, 0, NULL)  = 5002
```

**Analysis:**

**1. Parent creates pipe (PID 5000):**
```
pipe([3, 4]) = 0
```
- Creates pipe with fds 3 and 4
- fd=3 is read end, fd=4 is write end
- Both ends available in parent

**2. Parent forks first child (echo):**
```
fork() = 5001
```
- Child (PID 5001) inherits pipe fds 3 and 4

**3. First child redirects stdout:**
```
[pid 5001] dup2(4, 1) = 1
```
- Child duplicates fd=4 to fd=1 (stdout)
- stdout now writes to pipe write end
- echo's output will go into pipe

**4. First child closes pipes:**
```
[pid 5001] close(3) = 0   # Close unused read end
[pid 5001] close(4) = 0   # Close original write end
```
- Closes read end (won't read from pipe)
- Closes original write end (dup still exists on fd=1)
- Clean up - only keep what's needed

**5. Parent forks second child (cat):**
```
fork() = 5002
```
- Child (PID 5002) inherits pipe fds 3 and 4

**6. Second child redirects stdin:**
```
[pid 5002] dup2(3, 0) = 0
```
- Child duplicates fd=3 to fd=0 (stdin)
- stdin now reads from pipe read end
- cat will read input from pipe

**7. Second child closes pipes:**
```
[pid 5002] close(3) = 0   # Close original read end
[pid 5002] close(4) = 0   # Close unused write end
```
- Closes original read end (dup still exists on fd=0)
- Closes write end (won't write to pipe)

**8. Parent closes all pipes:**
```
[pid 5000] close(3) = 0
[pid 5000] close(4) = 0
```
- Parent doesn't need pipes
- Critical: must close write end so cat gets EOF!
- If parent kept write end open, cat would hang

**9. Parent waits for children:**
```
wait4(-1, NULL, 0, NULL) = 5001
wait4(-1, NULL, 0, NULL) = 5002
```
- Waits for first child (echo) to complete
- Waits for second child (cat) to complete

**Verification:**
- ✓ One pipe created (correct for 2 commands)
- ✓ Two children forked
- ✓ First child: stdout → pipe write
- ✓ Second child: stdin ← pipe read
- ✓ Both children close all pipes
- ✓ Parent closes all pipes
- ✓ Parent waits for both children

**Why closing matters:**
If parent didn't close fd=4, cat would never get EOF because:
- cat reads from fd=3 (pipe read end)
- EOF only comes when ALL write ends (fd=4) are closed
- Parent still has fd=4 open
- cat blocks forever waiting for more input

---

## Resources

- `man 2 pipe` - pipe system call
- `man 2 dup2` - duplicate file descriptor
- `man 2 close` - close file descriptor
- `man strace` - strace documentation
- Your AI tool of choice (ChatGPT, Claude, Gemini, etc.)

---

## Final Thought

Pipes are invisible - you can't see the data flowing between commands. strace makes them visible by showing the actual system calls:

1. pipe() creates the channel
2. dup2() redirects stdin/stdout
3. close() cleans up unused ends
4. execve() runs the command
5. Data flows automatically!

The goal isn't just to trace some system calls - it's to **understand how your shell connects processes together** at the deepest level. This knowledge applies to every Unix/Linux system you'll ever work with.

**Good luck with your analysis!**

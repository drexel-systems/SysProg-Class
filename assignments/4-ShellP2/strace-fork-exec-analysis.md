# System Call Analysis with strace: Fork/Exec

**Assignment Component:** Required (10 points)  
**Difficulty:** Intermediate - Requires Independent Research  
**Skills:** Process Tracing, Fork/Exec Understanding, Self-Directed Learning

---

## The Challenge

You've implemented fork/exec to run commands in your shell. But how do you **prove** your implementation is correct? How do you see what's actually happening at the operating system level when processes are created and replaced?

**Your task:** Use `strace` to trace and analyze the system calls your shell makes when executing commands. Verify fork/exec works correctly, understand parent/child behavior, and document what you discover.

**Specifically, you need to:**
1. Learn how to use `strace` for process tracing
2. Trace your shell's fork/exec operations
3. Analyze parent vs child process behavior
4. Investigate how execvp() searches PATH
5. Document your findings and learning process using AI tools

**The approach:** Use AI tools (ChatGPT, Claude, Gemini, etc.) to research `strace` independently. This is a required component, not extra credit.

---

## Why This Matters

**In systems programming:**
- Your code might compile and seem to work, but are processes created correctly?
- `strace` lets you see the actual system calls (fork, execvp, waitpid)
- It's the definitive way to debug process behavior
- Essential for understanding how Unix process creation works

**Professional reality:**
- Every systems programmer uses `strace` for debugging
- It's the standard tool for tracing system calls
- You'll use it throughout your career for systems troubleshooting
- Understanding fork/exec is crucial for all Unix programming

**For this assignment:**
- Validates your fork() creates child correctly
- Shows how execvp() searches PATH directories
- Proves waitpid() synchronization works
- Helps debug if commands aren't executing

---

## Getting Started: Key Questions to Explore

Use AI tools to research and discover answers to these questions:

### Understanding Phase

1. **What is strace and what does it show?** How is it different from debugging with gdb?

2. **How do you trace a program with strace?** What's the basic syntax?

3. **How do you trace child processes?** By default strace only traces the parent!

4. **What does strace output format mean?** How do you read a line like:
   ```
   fork() = 1234
   ```

### Process Tracing Phase

5. **How do you see fork() in strace output?** What does it return in parent vs child?

6. **How do you trace both parent and child processes?** There's a special flag for this!

7. **What does execvp() look like in strace?** Can you see it searching PATH?

8. **How can you filter strace output** to show only fork, execvp, waitpid?

### Analysis Phase

9. **How can you tell parent from child in strace output?** What about PIDs?

10. **What happens when execvp() fails?** What system calls do you see?

11. **How does waitpid() appear in strace?** Can you see the exit code?

12. **How do you save strace output to a file?** You'll need it for your analysis.

---

## Learning Strategy: Using AI Effectively

### Research Approach

1. **Start broad**: "What is strace?" → "How do I trace processes?"
2. **Get specific**: "How do I trace child processes with strace?"
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
- fork() returns twice - once in parent, once in child
- execvp() replaces the process - you won't see anything after it
- Parent and child run concurrently - output may interleave

---

## What You Need to Deliver

### File: `strace-fork-exec-analysis.md`

Create this file in your assignment directory with the following sections:

### 1. Learning Process (2 points)

Document how you learned strace for process tracing:
- What AI tools did you use?
- What questions did you ask? (Include 3-4 specific prompts)
- What resources did the AI point you to?
- What challenges did you encounter learning strace for processes?

**Example:**
```
I used ChatGPT to learn strace for process tracing. I started by asking 
"How do I use strace to trace fork and exec system calls?" Then I asked 
"How do I trace child processes created by fork?" When I got output from 
both parent and child, I asked "How can I tell which PID is the parent 
and which is the child in strace output?"
```

### 2. Basic Fork/Exec Analysis (3 points)

Trace a simple command execution. For each part, provide strace output and analysis:

#### A. Executing a Simple Command

Run your shell and execute a simple command:
```bash
strace -f -e trace=fork,execve,wait4 ./dsh
dsh2> ls
dsh2> exit
```

**The `-f` flag is CRITICAL** - it traces child processes!

**Provide:**
- The strace output (focus on fork, execve, wait4)
- Identify the fork() call - what PID does it return?
- Identify the execve() call - note it's execve, not execvp (strace shows actual syscall)
- Identify the wait4() call - this is how waitpid() is implemented
- Verify parent waits for child

**Example analysis:**
```
System call sequence for "ls" command:

Parent Process (PID 1000):
1. fork() = 1001
   - Creates child process
   - Returns child PID (1001) to parent

Child Process (PID 1001):
2. execve("/usr/bin/ls", ["ls"], ...) = 0
   - Replaces child with ls program
   - Note: strace shows execve, but we called execvp
   - execvp searches PATH and calls execve
   - Success (returns 0)

Parent Process (PID 1000):
3. wait4(1001, [{WIFEXITED(s) && WEXITSTATUS(s)==0}], 0, NULL) = 1001
   - Waits for child 1001
   - Child exited normally with status 0
   - Returns child's PID (1001)
```

#### B. Command Not Found

Execute a command that doesn't exist:
```bash
strace -f -e trace=fork,execve,wait4 ./dsh
dsh2> notacommand
dsh2> exit
```

**Provide:**
- The strace output
- Note what execve() returns when command not found
- What error code does the child exit with?
- How does parent handle this?

#### C. Command with Arguments

Execute a command with arguments:
```bash
strace -f -e trace=fork,execve,wait4 ./dsh
dsh2> echo "hello world"
dsh2> exit
```

**Provide:**
- The strace output
- Show how arguments are passed to execve()
- Note the format of argv array

### 3. PATH Search Investigation (3 points)

This is the most interesting part! Investigate how execvp() searches PATH.

#### A. Full Trace of PATH Search

Run with ALL syscalls to see PATH search:
```bash
strace -f ./dsh 2>&1 | grep -A 20 "execve"
```

Or save to file:
```bash
strace -f -o trace.txt ./dsh
# Then run: ls
# Then run: exit
# Look at trace.txt
```

**Answer these questions:**

1. **What system calls does execvp() make before calling execve()?**
   - Hint: Look for `access()` or `stat()` calls
   - These check if file exists

2. **How many directories does it check?**
   - Count the failed execve() or access() calls
   - Each directory in PATH is checked

3. **What error does execve() return when file not found?**
   - Look for ENOENT (file not found)
   - See the sequence of failures

4. **Which directory finally succeeds?**
   - The last execve() that returns 0
   - Example: `/usr/bin/ls`

#### B. Understanding the Search

Based on your investigation, explain:
- Why does execvp() try multiple directories?
- What is the PATH environment variable?
- How does your shell find commands without absolute paths?
- What would happen if command wasn't in any PATH directory?

### 4. Parent/Child Process Verification (2 points)

Verify your fork/exec implementation is correct by checking:

**Checklist:**
- [ ] fork() is called exactly once per command
- [ ] fork() returns different values in parent and child
- [ ] Child calls execve() (from your execvp())
- [ ] Parent calls wait4() (from your waitpid())
- [ ] Parent waits AFTER fork, not before
- [ ] Child process PID matches what parent got from fork()

**Questions to answer:**
1. Does your implementation create the child process correctly?
2. Does the child replace itself with the command?
3. Does the parent wait for the child to complete?
4. Are there any unexpected system calls?

If you found bugs, describe what was wrong and how you fixed it.

---

## Technical Requirements

### strace Commands to Use

**Basic tracing with child processes:**
```bash
strace -f -e trace=fork,execve,wait4 ./dsh
```

**Save output to file:**
```bash
strace -f -e trace=fork,execve,wait4 -o trace.txt ./dsh
```

**See all syscalls (verbose):**
```bash
strace -f ./dsh 2>&1 | tee full_trace.txt
```

**Filter for specific command:**
```bash
strace -f ./dsh 2>&1 | grep -E "(fork|execve|wait4)"
```

### Key strace Flags

- `-f` : Follow child processes (ESSENTIAL for fork!)
- `-e trace=fork,execve,wait4` : Only show these syscalls
- `-o FILE` : Save output to file
- `2>&1` : Redirect stderr to stdout (strace writes to stderr)

### Understanding strace Output

**System call format:**
```
syscall_name(arg1, arg2, ...) = return_value
```

**Parent fork example:**
```
[pid 1000] fork() = 1001
```
- Parent (PID 1000) called fork()
- Returned child PID 1001

**Child fork example:**
```
[pid 1001] execve("/bin/ls", ["ls"], ...) = 0
```
- Child (PID 1001) called execve()
- Executed /bin/ls successfully (return 0)

**Parent wait example:**
```
[pid 1000] wait4(1001, [{WIFEXITED(s) && WEXITSTATUS(s)==0}], 0, NULL) = 1001
```
- Parent waited for child 1001
- Child exited normally with status 0

---

## Grading Rubric

**10 points total:**

**Learning Process (2 points)**
- 2 pts: Clear documentation of AI-assisted learning with specific examples
- 1 pt: Vague description of learning process
- 0 pts: No evidence of learning process

**Basic Fork/Exec Analysis (3 points)**
- 3 pts: All three scenarios traced and analyzed correctly
- 2 pts: Two scenarios analyzed well
- 1 pt: One scenario analyzed
- 0 pts: No meaningful analysis

**PATH Search Investigation (3 points)**
- 3 pts: Thorough investigation with correct explanations
- 2 pts: Good investigation, minor gaps in understanding
- 1 pt: Basic investigation, significant gaps
- 0 pts: No investigation or incorrect

**Process Verification (2 points)**
- 2 pts: Thorough verification, identifies implementation correctness
- 1 pt: Basic verification, incomplete
- 0 pts: No verification or incorrect

---

## Hints for Success

### Running strace on Your Shell

**Your shell needs the `-f` flag:**
```bash
strace -f ./dsh        # Correct - traces child processes
strace ./dsh           # Wrong - only traces parent shell
```

**Test with simple command:**
```bash
strace -f -e trace=fork,execve,wait4 ./dsh
dsh2> ls
dsh2> exit
```

### Understanding Fork Return Values

**In parent process:**
```
fork() = 1234    # Returns child's PID
```

**In child process:**
```
fork() = 0       # Returns 0 to child
```

### Finding the execve() Call

**After fork, look for:**
```
[pid 1234] execve("/usr/bin/ls", ["ls"], ...) = 0
```

This is your child process executing the command!

### Common Issues

**Can't see child process:**
- Did you use `-f` flag?
- Check if fork() is actually being called

**Too much output:**
- Use `-e trace=fork,execve,wait4` to filter
- Save to file and search: `grep execve trace.txt`

**PIDs are confusing:**
- Parent keeps its original PID
- Child gets new PID (returned by fork to parent)
- Child's fork() returns 0 (not its PID!)

---

## Example: What Good Analysis Looks Like

Here's what a strong fork/exec analysis might include:

### Executing "ls" Command

**Command:**
```bash
strace -f -e trace=fork,execve,wait4 ./dsh
dsh2> ls
dsh2> exit
```

**Output:**
```
[pid 5000] fork()                        = 5001
[pid 5001] execve("/usr/bin/ls", ["ls"], ...) = 0
[pid 5000] wait4(5001, [{WIFEXITED(s) && WEXITSTATUS(s)==0}], 0, NULL) = 5001
```

**Analysis:**

**1. Fork System Call:**
```
[pid 5000] fork() = 5001
```
- Parent shell (PID 5000) calls fork()
- New child process created with PID 5001
- Parent receives child's PID (5001) as return value
- Child also created but its return value (0) not shown in this line

**2. Child Executes Command:**
```
[pid 5001] execve("/usr/bin/ls", ["ls"], ...) = 0
```
- Child process (PID 5001) calls execve()
- Replaces itself with /usr/bin/ls program
- Arguments passed: ["ls"]
- Success (return value 0)
- Child's memory now contains ls program, not shell

**3. Parent Waits:**
```
[pid 5000] wait4(5001, [{WIFEXITED(s) && WEXITSTATUS(s)==0}], 0, NULL) = 5001
```
- Parent (PID 5000) waits for child 5001
- WIFEXITED(s) means child exited normally
- WEXITSTATUS(s)==0 means exit code was 0 (success)
- wait4() returns child's PID (5001)

**Verification:**
- ✓ Fork creates child correctly (returns PID to parent)
- ✓ Child calls execve to run command
- ✓ Parent waits for child using wait4
- ✓ All PIDs match correctly
- ✓ Exit code 0 indicates success

---

## Resources

- `man strace` - comprehensive strace documentation
- `man 2 fork` - fork system call
- `man 3 execvp` - execvp library call
- `man 2 execve` - actual execve system call
- `man 2 waitpid` - waitpid call (uses wait4 syscall)
- Your AI tool of choice (ChatGPT, Claude, Gemini, etc.)

---

## Final Thought

strace shows you **ground truth** - the actual system calls your program makes at the OS level. When you run `ls` in your shell:

1. Your shell calls fork() - creates child
2. Child calls execve() - becomes ls
3. ls does its work (not shown - we filter syscalls)
4. ls exits
5. Parent's wait4() returns

This happens EVERY TIME you run ANY command in ANY shell. You're seeing the fundamental mechanism of Unix process creation!

The goal isn't just to trace some system calls - it's to **understand how your shell creates and manages processes** at the deepest level.

**Good luck with your analysis!**
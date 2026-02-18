# Assignment: Custom Shell Part 3 - Pipes

This week we will implement **pipes** in our `dsh` Drexel Shell, allowing multiple commands to stream data between them. This is one of the most powerful features of Unix shells!

## What are Pipes?

Pipes connect the output of one command to the input of another, creating powerful data processing pipelines. When you run:

```bash
ls | grep txt | wc -l
```

This creates three processes:
1. `ls` lists files → output goes to pipe
2. `grep txt` reads from pipe, filters for "txt" → output goes to next pipe
3. `wc -l` reads from pipe, counts lines → output goes to terminal

Each command runs simultaneously, with data flowing through pipes from left to right!

## How Pipes Work

### The pipe() System Call

```c
int pipefd[2];
pipe(pipefd);
// pipefd[0] is read end
// pipefd[1] is write end
```

Creates a unidirectional data channel:
- Write to `pipefd[1]`
- Read from `pipefd[0]`
- Data flows from write end to read end

### The dup2() System Call

```c
dup2(oldfd, newfd);
```

Duplicates a file descriptor, redirecting I/O:
- `dup2(pipefd[0], STDIN_FILENO)` → stdin reads from pipe
- `dup2(pipefd[1], STDOUT_FILENO)` → stdout writes to pipe

### Pipe Plumbing

For `cmd1 | cmd2 | cmd3`:

1. **Create pipes:**
   - pipe1: connects cmd1 → cmd2
   - pipe2: connects cmd2 → cmd3

2. **Fork and setup cmd1:**
   - stdout → pipe1[1] (write end)
   - close unused pipe ends
   - execvp("cmd1")

3. **Fork and setup cmd2:**
   - stdin ← pipe1[0] (read end)
   - stdout → pipe2[1] (write end)
   - close unused pipe ends
   - execvp("cmd2")

4. **Fork and setup cmd3:**
   - stdin ← pipe2[0] (read end)
   - close unused pipe ends
   - execvp("cmd3")

5. **Parent closes all pipes and waits for all children**

![pipe diagram](pipes-diagram.png)

**Critical:** Close unused pipe ends! Otherwise:
- Readers block waiting for EOF that never comes
- File descriptor leaks
- Processes hang

---

## Reuse Prior Work!

**From Part 1:**
- Command list parsing (split by pipes)
- `command_list_t` structure
- Error handling for too many pipes

**From Part 2:**
- Fork/exec pattern
- Built-in commands (cd, exit)
- Quote handling

**New for Part 3:**
- Create pipes between commands
- Use dup2() to redirect stdin/stdout
- Close pipe ends properly
- Wait for all children

---

## Assignment Details

### Step 1 - Review [./starter/dshlib.h](./starter/dshlib.h)

The file now includes:
- `command_list_t` - back to handling multiple commands!
- `execute_pipeline()` - new function to implement
- `close_cmd_buff()` - helper for closing file descriptors

**Prompt changed to "dsh3>"** to indicate Part 3.

---

### Step 2 - Implement Pipeline Parsing in [./starter/dshlib.c](./starter/dshlib.c)

Reuse your Part 1 parsing code! Parse input into `command_list_t`:

```c
int build_cmd_list(char *cmd_line, command_list_t *clist)
```

**Requirements:**
- Split by pipe character `|`
- Each segment becomes a `cmd_buff_t`
- Handle up to `CMD_MAX` (8) commands
- Return `ERR_TOO_MANY_COMMANDS` if > 8 pipes

**Example:**
```
Input:  "ls -la | grep txt | wc -l"
Output: clist->num = 3
        clist->commands[0] = ["ls", "-la"]
        clist->commands[1] = ["grep", "txt"]
        clist->commands[2] = ["wc", "-l"]
```

---

### Step 3 - Implement Pipeline Execution in [./starter/dshlib.c](./starter/dshlib.c)

Implement `execute_pipeline()` to run piped commands.

**Function signature:**
```c
int execute_pipeline(command_list_t *clist);
```

**Algorithm for N commands:**

```c
// 1. Create N-1 pipes (one less than number of commands)
int pipes[N-1][2];
for (int i = 0; i < N-1; i++) {
    pipe(pipes[i]);
}

// 2. Fork and setup each command
for (int i = 0; i < N; i++) {
    pid_t pid = fork();
    
    if (pid == 0) {  // Child process
        // Setup stdin (if not first command)
        if (i > 0) {
            dup2(pipes[i-1][0], STDIN_FILENO);
        }
        
        // Setup stdout (if not last command)
        if (i < N-1) {
            dup2(pipes[i][1], STDOUT_FILENO);
        }
        
        // Close ALL pipe file descriptors
        for (int j = 0; j < N-1; j++) {
            close(pipes[j][0]);
            close(pipes[j][1]);
        }
        
        // Execute command
        execvp(clist->commands[i].argv[0], clist->commands[i].argv);
        exit(EXIT_FAILURE);
    }
}

// 3. Parent closes all pipes
for (int i = 0; i < N-1; i++) {
    close(pipes[i][0]);
    close(pipes[i][1]);
}

// 4. Wait for all children
for (int i = 0; i < N; i++) {
    wait(NULL);
}
```

**Key points:**
- Create pipes BEFORE forking
- Each child closes ALL pipe ends after dup2
- Parent closes all pipes before waiting
- Wait for ALL children (not just one)

---

### Step 4 - Handle Single Command vs Pipeline

In your main loop, detect single command vs pipeline:

```c
if (clist.num == 1) {
    // Single command - use Part 2 exec_cmd()
    exec_cmd(&clist.commands[0]);
} else {
    // Pipeline - use execute_pipeline()
    execute_pipeline(&clist);
}
```

This keeps single commands simpler (no pipe overhead).

---

### Step 5 - Implement Built-in Commands

Built-in commands should NOT be piped:
- If command list starts with built-in, execute it (no piping)
- If built-in appears in middle of pipeline, that's an error (but we'll simplify: just execute it)

**Why?** Built-ins modify the shell's state. They can't be piped like external commands.

---

### Step 6 - System Call Analysis with strace

**Points: 10 (REQUIRED)**

Once your pipeline implementation works, you must analyze pipe behavior using `strace` to understand how pipes and file descriptor manipulation work at the OS level.

**What You'll Do:**
1. Use AI tools to learn `strace` for pipe/dup2 tracing
2. Trace your shell's pipe operations
3. Analyze pipe() creating file descriptors
4. Investigate dup2() redirecting stdin/stdout
5. Understand close() and file descriptor leaks
6. Document your findings and learning process

**Why This Matters:**
- **Validates pipe implementation**: See actual pipe() and dup2() calls
- **Understand file descriptors**: See how fds are created and manipulated
- **Prevent descriptor leaks**: See why closing pipes is critical
- **Professional skill**: Every systems programmer needs to understand pipes

**Deliverable:**
Create a file `strace-pipe-analysis.md` following the detailed instructions in [strace-pipe-analysis.md](strace-pipe-analysis.md).

**What You'll Analyze:**
- pipe() system calls creating fd pairs
- dup2() redirecting stdin/stdout
- close() closing unused pipe ends
- Multiple child processes running concurrently
- Data flowing through pipes

See [strace-pipe-analysis.md](strace-pipe-analysis.md) for complete instructions on this required component.

---

### Sample Run with Sample Output

```bash
./dsh
dsh3> ls | grep makefile
makefile
dsh3> ls | grep txt | wc -l
3
dsh3> echo "hello world" | cat
hello world
dsh3> cat makefile | head -5 | tail -2
CC = gcc
CFLAGS = -Wall -Wextra -g
dsh3> ps aux | grep dsh
user     1234  0.0  0.1  ... ./dsh
dsh3> pwd
/home/user/04-ShellP3
dsh3> cd ..
dsh3> pwd
/home/user
dsh3> exit
exiting...
cmd loop returned 0
```

**Note:** Pipes actually work! Data flows between commands!

---

## Extra Credit: +15

Implement I/O redirection with `>`, `<`, and `>>`.

**Requirements:**

1. **Output redirection (>)** - Write stdout to file
   ```bash
   ls > files.txt
   ```

2. **Input redirection (<)** - Read stdin from file
   ```bash
   cat < input.txt
   ```

3. **Append redirection (>>)** - Append stdout to file
   ```bash
   echo "more" >> log.txt
   ```

4. **Combine with pipes**
   ```bash
   cat file.txt | grep keyword > results.txt
   ```

**Implementation hints:**
- Detect redirection operators when parsing
- Store filename and redirection type in cmd_buff
- Before execvp(), open file and dup2() to appropriate fd
- Use `O_WRONLY|O_CREAT|O_TRUNC` for `>`
- Use `O_WRONLY|O_CREAT|O_APPEND` for `>>`
- Use `O_RDONLY` for `<`

**System calls:**
```c
// For >
int fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0644);
dup2(fd, STDOUT_FILENO);
close(fd);

// For <
int fd = open(filename, O_RDONLY);
dup2(fd, STDIN_FILENO);
close(fd);

// For >>
int fd = open(filename, O_WRONLY|O_CREAT|O_APPEND, 0644);
dup2(fd, STDOUT_FILENO);
close(fd);
```

---

## Grading Rubric

This assignment will be weighted **90 points**.

- **55 points**: Correct implementation of required functionality
  - Pipeline execution (25 points)
  - Pipe setup and dup2 (15 points)
  - Closing file descriptors (10 points)
  - Waiting for all children (5 points)
- **5 points**: Code quality (readable, well-commented, good design)
- **10 points**: strace pipe analysis (`strace-pipe-analysis.md`)
- **15 points**: [EXTRA CREDIT] I/O redirection (>, <, >>)

**Total points achievable: 90/70**

---

## Submission Requirements

All files for this assignment should be placed in the `04-ShellP3` directory in your GitHub Classroom repository.

**Required Files:**
1. `dshlib.c` - Your implementation
2. `dshlib.h` - If you added any functions
3. `dsh_cli.c` - Should not need changes
4. `strace-pipe-analysis.md` - Your strace analysis
5. All provided files (`makefile`, etc.)

**Submission Process:**

1. Ensure all files are in `04-ShellP3/` directory
2. Test compilation: `make clean && make`
3. Test functionality: `pytest test_dsh3.py -v` (all tests must pass)
4. Commit and push:
   ```bash
   git add 04-ShellP3/
   git commit -m "Complete shell part 3"
   git push origin main
   ```
5. Submit repository URL on Canvas

**Note:** We will clone your repository and grade the contents of your `04-ShellP3` directory.

---

## Testing

Your implementation will be tested using pytest. Install pytest with:

```bash
pip3 install pytest --break-system-packages
# or
make install-pytest
```

Run tests with:

```bash
pytest test_dsh3.py -v
# or
make test
```

All tests must pass for full credit!

**Tests verify:**
- Single command execution (from Part 2)
- Two-command pipes work
- Three-command pipes work
- Multiple pipes work
- cd and built-ins work
- Pipe actually transfers data

---

## Important Concepts

### Why Close Pipe Ends?

**Problem:** If you don't close unused pipe ends:
- Reader blocks waiting for EOF
- EOF only comes when ALL write ends are closed
- If parent keeps write end open, child never gets EOF
- Process hangs!

**Solution:** Close every pipe end you don't need:
- Child closes all pipes after dup2()
- Parent closes all pipes after forking children

### File Descriptor Lifecycle

```
1. pipe(pipefd) → creates pipefd[0] and pipefd[1]
2. fork() → child inherits both ends
3. dup2(pipefd[1], STDOUT) → stdout now writes to pipe
4. close(pipefd[0]) → close unused read end
5. close(pipefd[1]) → close original write end (dup exists)
6. execvp() → inherited dup'd fd still works
```

### Why Wait for All Children?

```c
// WRONG - only waits for one child
wait(NULL);

// RIGHT - waits for ALL children
for (int i = 0; i < num_commands; i++) {
    wait(NULL);
}
```

If you don't wait for all, zombie processes accumulate!

---

## Tips for Success

### Start Simple

1. **First:** Get two-command pipe working
   ```bash
   ls | cat
   ```

2. **Then:** Add three commands
   ```bash
   ls | cat | cat
   ```

3. **Finally:** Support arbitrary number

### Debug with printf

```c
printf("Creating pipe %d\n", i);
pipe(pipes[i]);

printf("Forking child %d\n", i);
pid_t pid = fork();

printf("Child %d: setting up stdin/stdout\n", i);
// ... dup2 calls ...

printf("Child %d: closing pipes\n", i);
// ... close calls ...
```

### Test Incrementally

```bash
# Test single command (should still work)
dsh3> ls

# Test simple pipe
dsh3> ls | cat

# Test data actually flows
dsh3> echo hello | cat

# Test filtering
dsh3> ls | grep makefile

# Test multiple pipes
dsh3> ls | cat | cat | cat
```

### Common Mistakes

**1. Not closing pipes in parent:**
```c
// Parent must close all pipes!
for (int i = 0; i < num_pipes; i++) {
    close(pipes[i][0]);
    close(pipes[i][1]);
}
```

**2. Not closing pipes in child:**
```c
// Child must close ALL pipes after dup2
for (int j = 0; j < num_pipes; j++) {
    close(pipes[j][0]);
    close(pipes[j][1]);
}
```

**3. Only waiting for one child:**
```c
// Need to wait for EACH child
for (int i = 0; i < num_commands; i++) {
    wait(NULL);
}
```

**4. Forgetting to dup2 before close:**
```c
// WRONG order
close(pipefd[1]);
dup2(pipefd[1], STDOUT_FILENO);  // Too late!

// RIGHT order
dup2(pipefd[1], STDOUT_FILENO);
close(pipefd[1]);
```

---

## Understanding Pipes Deeply

### Why Pipes Are Powerful

Pipes enable **composition** - combining simple tools to solve complex problems:

```bash
# Count .txt files
ls | grep txt | wc -l

# Find largest files
ls -la | sort -k 5 -n | tail -5

# Process logs
cat access.log | grep "ERROR" | cut -d' ' -f1 | sort | uniq -c
```

This is the **Unix philosophy**: Small tools that do one thing well, composed via pipes.

### Pipe Capacity

Pipes have limited buffer (typically 64KB):
- Writer blocks if pipe full
- Reader blocks if pipe empty
- This provides flow control automatically!

### Pipe vs File

**Pipe:**
- Unidirectional
- In-memory buffer
- Producer/consumer run concurrently
- Data disappears after read

**File:**
- Can read/write/seek
- Persistent on disk
- Can read multiple times

Pipes are perfect for streaming data between processes!

---

Good luck! Pipes are the heart of Unix shell power. Once you implement this, you'll understand how every `command1 | command2` pipeline in Unix works!

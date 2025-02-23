# Assignment: Custom Shell Part 3 - Pipes

This week we will build on our `dsh` Drexel Shell by implementing pipes. In the first shell assignment we approached this subject by splitting lines of input by the pipe `|` and printing out each separate command parsed between the pipes. In the second assignment we set the pipe splitting logic aside and implemented _execution_ of single commands.

This week we'll bring these concepts together by implementing pipelined execution of multiple commands!

# Reuse Prior Work!

The `dsh` assignments are meant to be additive. This week you'll need to use code you wrote in the first two shell assignments. Be sure to re-use your existing code, and refactor it to meet the requirements of this assignment.

# Pipes

This week you'll need to use a couple new important syscalls - `pipe()` and `dup2()`.

The `pipe()` command creates a 1-way communication path that is implemented with two file descriptions. Conceptually it looks like this:

```txt
  process-1-output --write--> pipe[fd1]:pipe[fd0] --read--> process-2-input
```

How do the pipes get "connected" between the processes? That's where `dup2()` comes in.

> Tip: pipes should be created and connected inside a `fork()`; you've already used `fork/execvp` to implement a single command. This time you'll need to do that in a loop, because each command should get it's own `fork/execvp`. So, you'll need to handle `pipe()` and `dup2()` _inside_ each child's fork.

Think of `dup2()` as sort of patching or redirection. The function signature is:

    `dup2(source_fd, target_fd)`

Remember that each forked child has it's own process space and set of file descriptors, to include `STDIN_FILENO` and `STDOUT_FILENO`. So, inside each child process, you could use dup2 to _replace_ the child's STDIN and STDOUT with pipe file descriptors instead.

**Preventing Descriptor Leaks**

When you use `dup2()`, it _copies_ the source FD onto the target, effectively replacing the target. The source is no longer used, and can be closed. If you don't close the original pipes after copying with `dup2()`, you have introduced a **descriptor leak**; this could cause your program to run out of available file descriptors.

# Multiple Children / Waitpid

Since you must create multiple forks to handle piped commands, be sure to wait on all of them with `waitpid()`, which can be used to wait for a specific process id. _(Hint: you'll need to save the pid of each process you fork)._

# Assignment Details

### Step 1 - Review [./starter/dshlib.h](./starter/dshlib.h)

The file [./starter/dshlib.h](./starter/dshlib.h) contains some useful definitions and types. Review the available resources in this file before you start coding - these are intended to make your work easier and more robust!

### Step 2 - Re-implement Your Main Loop and Parsing Code in exec_local_cmd_loop() to parse multiple commands by pipe [./starter/dshlib.c](./starter/dshlib.c)

This should mostly be a refactoring, copy/paste exercise to merge your solutions from the first two shell assignments. You need to combine your pipe tokenizing solution with loading up `cmd_buff_t` ultimately into a `command_list_t`.

### Step 3 - Implement pipelined execution of the parsed commands [./starter/dshlib.c](./starter/dshlib.c)

The first part of this is a refactoring problem - instead of just one `fork/exec`, you'll need to do that in a loop and keep track of the child pids and child exit statuses.

The second part is implementing pipe logic. The section above named "Pipes" contains everything you really need to know! Also, check out demo code from this week's lecture [5-pipe-handling](https://github.com/drexel-systems/SysProg-Class/tree/main/demos/process-thread/5-pipe-handling). 


### Step 4 - Create BATS Tests

You should now be familiar with authoring your own [bash-based BATS unit tests](https://bats-core.readthedocs.io/en/stable/tutorial.html#your-first-test) from the prior assignment.

Same as last week, you have an "assignment-tests.sh" file that we provide and you cannot change, and a "student_tests.sh" file that **you need to add your own tests to**:

- your-workspace-folder/
  - bats/assignement_tests.sh
  - bats/student_tests.sh

**This week we expect YOU to come up with the majority of the test cases! If you have trouble, look at the tests we provided from prior weeks**

**Be sure to run `make test` and verify your tests pass before submitting assignments.**

### Step 5 - Answer Questions

Answer the questions located in [./questions.md](./questions.md).

### Sample Run with Sample Output
The below shows a sample run executing multiple commands and the expected program output:

```bash
./dsh
dsh3> ls | grep ".c" 
dragon.c
dsh_cli.c
dshlib.c
dsh3> exit
exiting...
cmd loop returned 0
```

### Extra Credit: +10

Last week you should have learned about redirection (it was part of the assignment's research question). Once you've implemented pipes for this assignment, redirection is a natural next step.

**Requirements**

- parse `<` and `>` during command buffer creation; you'll need to parse and track these on `cmd_buff_t`
- modify command execution to use `dup2()` in a similar way as you did for pipes; you can copy the in and out fd's specified by the user on to STDIN or STDOUT of the child's forked process

Example run:

```bash
./dsh
dsh3> echo "hello, class" > out.txt
dsh3> cat out.txt
hello, class
```

### Extra Credit++: +5

Extend the first extra credit to implement `>>`. This is the same as `>`, except the target fd is in append mode.

Example run:

```bash
./dsh
dsh3> echo "hello, class" > out.txt
dsh3> cat out.txt
hello, class
dsh3> echo "this is line 2" >> out.txt
dsh3> cat out.txt
hello, class
this is line 2
dsh3> 
```

#### Grading Rubric

- 50 points: Correct implementation of required functionality
- 5 points:  Code quality (how easy is your solution to follow)
- 10 points: Answering the written questions: [questions.md](./questions.md)
- 10 points: Quality and breadth of BATS unit tests
- 10 points: [EXTRA CREDIT] handle < and > redirection
- 5 points: [EXTRA CREDIT++] handle >> append redirection

Total points achievable is 90/75.



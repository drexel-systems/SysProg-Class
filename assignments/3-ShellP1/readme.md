# Assignment: Custom Shell Part 1 - Command Line Parser

This week we will begin the first of a multi-part assignment to build a **custom shell** called `dsh` (Drexel shell).

## What is a Shell?

A "shell" is a type of user interface for interacting with an operating system. You are already familiar with Linux command line shells in this course - the integrated terminal in vscode runs a shell (probably "bash" if you are using any of the Linux virtualization options suggested).

When we say your terminal "runs the bash shell" it means this: a shell is a generalized term for a binary that provides a terminal-based interface; "bash" is the name of a specific shell that you can install and run as a binary. Linux distributions have default shell configurations, and most of them default to `bash`. You can install new shells and use them as your default shell upon login; `zsh` is a popular shell that many users prefer over bash.

The purpose of the shell is to broker inputs and outputs to built-in shell commands, other binaries, and kernel operations like syscalls. When you open a terminal in vscode, the shell is interactive and you must provide inputs by typing in the terminal. Shells are also a part of "headless" processes like cron jobs (in Linux cron jobs run binaries on a schedule automatically, even if you are not logged in); these background jobs still execute in the context of a shell. The shell provides the job the same interface to the operating system, and the job can use the output that the shell brokers.

## Shell Built-in vs. External Commands

Most "commands" you run are not implemented in the shell logic; they are usually other binaries on the filesystem that are invoked when you refer to them from your shell.

However ... there are several (50+) commands that are built in to the logic of the shell itself. Here are some examples:

* `cd`: change the current working directory
* `exit`: exit from the shell process
* `echo`: print text to STDOUT
* `export`: set environment variable in the current shell

In this assignment, we will implement one "built in" command: `exit`. There is also an optional `dragon` command for extra credit.

Future assignments will generally follow the pattern of other popular shells and implement some of the common builtin commands.

## What Else Does the Shell Do?

Beyond implementing built-in commands, a shell acts as a robust broker of input and output. Shells must provide a lot of "glue" to handle streams; for example this is a common sequence you might see in a linux shell: 

```sh
run-some-command | grep "keyword"
```

The `|` symbol is called a pipe, and it serves to stream the output of the first command (`run-some-command`) into the input of the second command (`grep`). We'll implement much of this "glue" logic in future assignments in this course.

## Assignment Details

In this assignment you will implement the first part of `dsh`: a command line parser to interpret commands and their arguments.

You will not implement any command logic, other than exiting when the `exit` command is provided. There is also one optional extra credit for implementing the `dragon` command.

**This assignment has two main components:**
1. **Implementation** (35 points): Build the command parser in C
2. **AI Investigation** (15 points): Research shell fundamentals and BusyBox using AI tools (see [shell-fundamentals.md](shell-fundamentals.md))

---

### Step 1 - Review [./starter/dshlib.h](./starter/dshlib.h)

The file [./starter/dshlib.h](./starter/dshlib.h) contains some useful definitions and types. Review the available resources in this file before you start coding - these are intended to make your work easier and more robust!

**Key data structures you'll use:**

**`cmd_buff_t`** - Represents a single parsed command:
```c
typedef struct cmd_buff {
    int  argc;              // Number of arguments (including command)
    char *argv[CMD_ARGV_MAX]; // Array of argument strings
    char *_cmd_buffer;      // Internal buffer (you manage this)
} cmd_buff_t;
```

Example: `"ls -la /tmp"` becomes:
- `argc = 3`
- `argv[0] = "ls"`
- `argv[1] = "-la"`
- `argv[2] = "/tmp"`
- `argv[3] = NULL`

**`command_list_t`** - Contains multiple commands split by pipes:
```c
typedef struct command_list {
    int num;                    // Number of commands
    cmd_buff_t commands[CMD_MAX]; // Array of commands
} command_list_t;
```

Example: `"ls | grep txt | wc -l"` becomes:
- `num = 3`
- `commands[0]` = `["ls"]`
- `commands[1]` = `["grep", "txt"]`
- `commands[2]` = `["wc", "-l"]`

---

### Step 2 - Implement [./starter/dsh_cli.c](./starter/dsh_cli.c)

This contains the entrypoint of your shell. **Good news: You don't need to edit this file!** All implementation goes in `dshlib.c`.

The main loop is already provided - it calls `exec_local_cmd_loop()` which you'll implement.

---

### Step 3 - Implement Main Loop in [./starter/dshlib.c](./starter/dshlib.c)

Implement `exec_local_cmd_loop()` which is the core of your shell.

**What it should do:**

1. **Loop forever** accepting user input
2. **Print the prompt** using `SH_PROMPT` constant
3. **Read a line of input** using `fgets()`
4. **Parse the input** into a `command_list_t`
5. **Handle the `exit` command** - break the loop and return
6. **Print the parsed commands** in the required format (see below)
7. **Free allocated memory** before looping back

**Starter code to get you going:**

```c
int exec_local_cmd_loop()
{
    char cmd_line[SH_CMD_MAX];
    command_list_t cmd_list;
    
    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_line, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }
        
        // Remove the trailing \n from cmd_line
        cmd_line[strcspn(cmd_line, "\n")] = '\0';
        
        // TODO: Check for exit command
        
        // TODO: Parse the command line into cmd_list
        
        // TODO: Print the parsed commands
        
        // TODO: Free memory
    }
    
    return OK;
}
```

**Helpful constants from `dshlib.h`:**
- `SH_PROMPT` - The shell prompt string ("dsh> ")
- `EXIT_CMD` - The exit command string ("exit")
- `SH_CMD_MAX` - Maximum command line length
- `CMD_MAX` - Maximum number of piped commands (8)
- `PIPE_CHAR` - The pipe character ('|')

**Return codes:**
- `OK` - Success
- `WARN_NO_CMDS` - Empty command line
- `ERR_TOO_MANY_COMMANDS` - More than 8 piped commands
- `ERR_MEMORY` - Memory allocation failure

**Console messages (use these constants):**
- `CMD_WARN_NO_CMD` - "warning: no commands provided\n"
- `CMD_ERR_PIPE_LIMIT` - "error: piping limited to %d commands\n"

---

### Step 3a - Implement the Built-in Function `exit`

Inside your main loop in `exec_local_cmd_loop()`, check for the `exit` command. When detected:

```c
if (strcmp(cmd_line, EXIT_CMD) == 0) {
    printf("exiting...\n");
    break;
}
```

This is how built-in commands work - they're handled by the shell itself, not executed as external programs.

---

### Step 4 - Implement Command Parsing in [./starter/dshlib.c](./starter/dshlib.c)

You need to implement `build_cmd_list()` to parse command lines:

```c
int build_cmd_list(char *cmd_line, command_list_t *clist)
```

**This function should:**

1. **Split the input by pipe characters** (`|`)
   - Use `strtok()` or manual parsing
   - Each segment between pipes is one command

2. **For each command segment:**
   - Allocate a `cmd_buff_t` using `alloc_cmd_buff()`
   - Parse into arguments (split by spaces)
   - Store in `clist->commands[i]`

3. **Handle edge cases:**
   - Empty input → return `WARN_NO_CMDS`
   - More than `CMD_MAX` commands → return `ERR_TOO_MANY_COMMANDS`
   - Leading/trailing spaces → trim them

4. **Set `clist->num`** to the number of commands parsed

**Parsing approach:**

```c
// Pseudocode for parsing
1. Check if cmd_line is empty or all whitespace
   - If yes, return WARN_NO_CMDS

2. Count how many pipe characters exist
   - If > CMD_MAX-1, return ERR_TOO_MANY_COMMANDS

3. Split cmd_line by '|' character
   - For each segment:
     a. Trim leading/trailing whitespace
     b. Split by spaces to get command and arguments
     c. Store in cmd_buff_t
     d. Add to command_list_t

4. Return OK
```

**Helper functions provided:**
- `alloc_cmd_buff()` - Allocate memory for a cmd_buff
- `free_cmd_buff()` - Free memory from a cmd_buff
- `build_cmd_buff()` - Parse a single command string into cmd_buff
- `free_cmd_list()` - Free entire command list

---

### Step 5 - Implement Printing Command List in [./starter/dshlib.c](./starter/dshlib.c)

After parsing, print the commands in the **exact format** shown below.

**Output format:**

```
PARSED COMMAND LINE - TOTAL COMMANDS <num>
<1> <command> [<args>]
<2> <command> [<args>]
...
```

**Rules:**
- First line uses `CMD_OK_HEADER` constant
- Each command is numbered starting from 1
- Arguments are in square brackets, space-separated
- Commands with no arguments don't have brackets

**Examples:**

Single command:
```
dsh> cmd
PARSED COMMAND LINE - TOTAL COMMANDS 1
<1> cmd
```

Command with arguments:
```
dsh> cmd_args a1 a2 -a3 --a4
PARSED COMMAND LINE - TOTAL COMMANDS 1
<1> cmd_args [a1 a2 -a3 --a4]
```

Multiple piped commands:
```
dsh> cmd1 | cmd2
PARSED COMMAND LINE - TOTAL COMMANDS 2
<1> cmd1
<2> cmd2
```

Piped commands with arguments:
```
dsh> cmda1 a1 a2 | cmda2 a3 a4 | cmd3 
PARSED COMMAND LINE - TOTAL COMMANDS 3
<1> cmda1 [a1 a2]
<2> cmda2 [a3 a4]
<3> cmd3
```

---

### Step 6 - Validate with Automated Testing

Test your implementation using pytest:

```bash
# Install pytest (first time only)
pip3 install pytest --break-system-packages

# Run all tests
pytest test_dsh.py -v

# Or use makefile
make test
```

**All tests must pass for full credit!**

Tests verify:
- Single commands
- Commands with arguments
- Piped commands
- Error handling (too many pipes, empty input)
- Exit command
- Edge cases (spaces, multiple pipes)

---

### Step 7 - Shell Fundamentals Investigation with AI

**Points: 15 (REQUIRED - Do not skip this!)**

Before you write code, you need to understand WHY shells exist and HOW they work conceptually. This investigation teaches you to use AI tools for technical research - a critical skill for software engineers.

**Complete instructions are in:** [shell-fundamentals.md](shell-fundamentals.md)

**What You'll Do:**
1. Use AI tools (ChatGPT, Claude, Gemini) to research shell concepts
2. Document your learning process (what prompts you used, what you discovered)
3. Explain shell fundamentals: purpose, parsing, built-ins vs external commands
4. Compare different shells (bash, zsh, fish)
5. Investigate BusyBox: what it is, why it exists, and how it differs from traditional shells

**Deliverable:** Create `shell-investigation.md` following the template and rubric provided in [shell-fundamentals.md](shell-fundamentals.md).

**Important:** Use proper markdown formatting! See the [GitHub Markdown Guide](https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax) for syntax reference.

**Why This Matters:** Understanding shell design will help you make better implementation decisions throughout all four shell assignments. This is required and worth 15 points - invest the time to do it well!

**What You'll Research:**
- What is a shell and why do operating systems need them?
- How do shells parse command lines (tokenization, quotes, metacharacters)?
- Built-in vs external commands (and why cd must be built-in)
- Different shells (bash, zsh, fish) and their design philosophies
- BusyBox: embedded systems approach to Unix utilities

See [shell-fundamentals.md](shell-fundamentals.md) for complete instructions, grading rubric, and examples of what strong research looks like.

---

### Sample Run with Sample Output

The below shows a sample run executing multiple commands and the expected program output:

```bash
./dsh
dsh> cmd
PARSED COMMAND LINE - TOTAL COMMANDS 1
<1> cmd
dsh> cmd_args a1 a2 -a3 --a4
PARSED COMMAND LINE - TOTAL COMMANDS 1
<1> cmd_args [a1 a2 -a3 --a4]
dsh> dragon
PARSED COMMAND LINE - TOTAL COMMANDS 1
<1> dragon
dsh> cmd1 | cmd2
PARSED COMMAND LINE - TOTAL COMMANDS 2
<1> cmd1
<2> cmd2
dsh> cmda1 a1 a2 | cmda2 a3 a4 | cmd3 
PARSED COMMAND LINE - TOTAL COMMANDS 3
<1> cmda1 [a1 a2]
<2> cmda2 [a3 a4]
<3> cmd3
dsh> 
warning: no commands provided
dsh> c1 | c2 | c3 | c4 | c5 | c6 | c7 | c8
PARSED COMMAND LINE - TOTAL COMMANDS 8
<1> c1
<2> c2
<3> c3
<4> c4
<5> c5
<6> c6
<7> c7
<8> c8
dsh> c1 | c2 | c3 | c4 | c5 | c6 | c7 | c8 | c9
error: piping limited to 8 commands
dsh> pipe1|pipe2|pipe3 |pipe4             
PARSED COMMAND LINE - TOTAL COMMANDS 4
<1> pipe1
<2> pipe2
<3> pipe3
<4> pipe4
dsh> pipe1|pipe2 |pipe3 pipe4| pipe5
PARSED COMMAND LINE - TOTAL COMMANDS 4
<1> pipe1
<2> pipe2
<3> pipe3 [pipe4]
<4> pipe5
dsh> exit
exiting...
cmd loop returned 0
```

---

## Extra Credit: +5

Add logic to detect the command "`dragon`" and print the Drexel dragon in ASCII art.

**Requirements:**
- Detect when user types exactly "dragon"
- Print the ASCII dragon (provided below)
- Continue shell loop after printing

Drexel dragon in ASCII with spaces preserved:

```
                                                                        @%%%%                       
                                                                     %%%%%%                         
                                                                    %%%%%%                          
                                                                 % %%%%%%%           @              
                                                                %%%%%%%%%%        %%%%%%%           
                                       %%%%%%%  %%%%@         %%%%%%%%%%%%@    %%%%%%  @%%%%        
                                  %%%%%%%%%%%%%%%%%%%%%%      %%%%%%%%%%%%%%%%%%%%%%%%%%%%          
                                %%%%%%%%%%%%%%%%%%%%%%%%%%   %%%%%%%%%%%% %%%%%%%%%%%%%%%           
                               %%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%     %%%            
                             %%%%%%%%%%%%%%%%%%%%%%%%%%%%@ @%%%%%%%%%%%%%%%%%%        %%            
                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%                
                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%              
                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%@              
      %%%%%%%%@           %%%%%%%%%%%%%%%%        %%%%%%%%%%%%%%%%%%%%%%%%%%      %%                
    %%%%%%%%%%%%%         %%@%%%%%%%%%%%%           %%%%%%%%%%% %%%%%%%%%%%%      @%                
  %%%%%%%%%%   %%%        %%%%%%%%%%%%%%            %%%%%%%%%%%%%%%%%%%%%%%%                        
 %%%%%%%%%       %         %%%%%%%%%%%%%             %%%%%%%%%%%%@%%%%%%%%%%%                       
%%%%%%%%%@                % %%%%%%%%%%%%%            @%%%%%%%%%%%%%%%%%%%%%%%%%                     
%%%%%%%%@                 %%@%%%%%%%%%%%%            @%%%%%%%%%%%%%%%%%%%%%%%%%%%%                  
%%%%%%%@                   %%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%              
%%%%%%%%%%                  %%%%%%%%%%%%%%%          %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%      %%%%  
%%%%%%%%%@                   @%%%%%%%%%%%%%%         %%%%%%%%%%%%@ %%%% %%%%%%%%%%%%%%%%%   %%%%%%%%
%%%%%%%%%%                  %%%%%%%%%%%%%%%%%        %%%%%%%%%%%%%      %%%%%%%%%%%%%%%%%% %%%%%%%%%
%%%%%%%%%@%%@                %%%%%%%%%%%%%%%%@       %%%%%%%%%%%%%%     %%%%%%%%%%%%%%%%%%%%%%%%  %%
 %%%%%%%%%%                  % %%%%%%%%%%%%%%@        %%%%%%%%%%%%%%   %%%%%%%%%%%%%%%%%%%%%%%%%% %%
  %%%%%%%%%%%%  @           %%%%%%%%%%%%%%%%%%        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %%% 
   %%%%%%%%%%%%% %%  %  %@ %%%%%%%%%%%%%%%%%%          %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    %%% 
    %%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%           @%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    %%%%%%% 
     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%              %%%%%%%%%%%%%%%%%%%%%%%%%%%%        %%%   
      @%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                  %%%%%%%%%%%%%%%%%%%%%%%%%               
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                      %%%%%%%%%%%%%%%%%%%  %%%%%%%          
           %%%%%%%%%%%%%%%%%%%%%%%%%%                           %%%%%%%%%%%%%%%  @%%%%%%%%%         
              %%%%%%%%%%%%%%%%%%%%           @%@%                  @%%%%%%%%%%%%%%%%%%   %%%        
                  %%%%%%%%%%%%%%%        %%%%%%%%%%                    %%%%%%%%%%%%%%%    %         
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                      %%%%%%%%%%%%%%            
                %%%%%%%%%%%%%%%%%%%%%%%%%%  %%%% %%%                      %%%%%%%%%%  %%%@          
                     %%%%%%%%%%%%%%%%%%% %%%%%% %%                          %%%%%%%%%%%%%@          
                                                                                 %%%%%%%@       
```

**Implementation Tips:**

1. **Using a string array:** Store the dragon in a const char* array in your code
2. **Using xxd command:** Generate a C array from the ASCII art
   ```bash
   # Save dragon to a file
   cat > dragon.txt << 'EOF'
   [paste dragon here]
   EOF
   
   # Convert to C array using xxd
   xxd -i dragon.txt > dragon.h
   
   # This creates an array like:
   # unsigned char dragon_txt[] = { 0x20, 0x20, ... };
   # unsigned int dragon_txt_len = 1234;
   ```
3. **Using a separate file:** Read from a file at runtime

**Example detection logic:**
```c
if (strcmp(cmd_list.commands[0].argv[0], "dragon") == 0) {
    // Print the dragon
    for (int i = 0; i < num_lines; i++) {
        printf("%s\n", dragon_lines[i]);
    }
    continue; // Skip normal command execution
}
```

**Tip:** The xxd approach is particularly useful because it preserves all spacing exactly as shown. You can investigate the `xxd` command using AI tools to learn how it works!

---

## Grading Rubric

This assignment will be weighted **55 points**.

- **30 points**: Correct implementation of required functionality
  - Command parsing (15 points)
  - Output formatting (10 points)
  - Exit command handling (5 points)
- **5 points**: Code quality (readable, well-commented, good design)
- **15 points**: Shell fundamentals investigation (`shell-investigation.md`)
  - Learning process (2 points)
  - Shell purpose and design (3 points)
  - Command line parsing (3 points)
  - Built-in vs external commands (2 points)
  - BusyBox investigation (3 points)
  - Markdown formatting (2 points)
- **5 points**: [EXTRA CREDIT] Implementation of the `dragon` command

**Total points achievable: 60/55**

---

## Submission Requirements

All files for this assignment should be placed in the `04-ShellP1` directory in your GitHub Classroom repository.

**Required Files:**
1. `dshlib.c` - Your implementation
2. `dshlib.h` - If you added any functions/constants
3. `dsh_cli.c` - Should not need changes
4. `shell-investigation.md` - Your AI investigation (following template in shell-fundamentals.md)
5. All provided files (`makefile`, etc.)

**Submission Process:**

1. Ensure all files are in `04-ShellP1/` directory
2. Test compilation: `make clean && make`
3. Test functionality: `pytest test_dsh.py -v` (all tests must pass)
4. Commit and push:
   ```bash
   git add 04-ShellP1/
   git commit -m "Complete shell part 1"
   git push origin main
   ```
5. Submit repository URL on Canvas

**Note:** We will clone your repository and grade the contents of your `04-ShellP1` directory.

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
pytest test_dsh.py -v
# or
make test
```

All tests must pass for full credit!

---

Good luck! Remember: focus on understanding the parsing logic. The execution part comes in the next assignment. Use AI tools to help you understand concepts, and don't hesitate to ask questions!
# Assignment: Custom Shell Part 1 - Command Line Parser

This week we will begin the first of a multi-part assignment to build a **custom shell** called `dsh` (Drexel shell).

# What is a Shell?

A "shell" is a type of user interface for interacting with an operating system. You are already familiar with Linux command line shells in this course - the integrated terminal in vscode runs a shell (probably "bash" if you are using any of the Linux virtualization options suggested).

When we say your terminal "runs the bash shell" it means this: a shell is a generalized term for a binary that provides a terminal-based interface; "bash" is the name of a specific shell that you can install and run as a binary. Linux distributions have default shell configurations, and most of them default to `bash`. You can install new shells and use them as your default shell upon login; `zsh` is a popular shell that many users prefer over bash.

The purpose of the shell is to broker inputs and outputs to built-in shell commands, other binaries, and kernel operations like syscalls. When you open a terminal in vscode, the shell is interactive and you must provide inputs by typing in the terminal. Shells are also a part of "headless" processes like cron jobs (in Linux cron jobs run binaries on a schedule automatically, even if you are not logged in); these background jobs still execute in the context of a shell. The shell provides the job the same interface to the operating system, and the job can use the output that the shell brokers.

# Shell built-in vs. external commands

Most "commands" you run are not implemented in the shell logic; they are usually other binaries on the filesystem that are invoked when you refer to them from your shell.

However ... there several (50+) commands that are built in to the logic of the shell itself. Here are some examples:

* cd: change the current working directory
* exit: exit from the shell process
* echo: print text to STDOUT
* export: set environment variable in the current shell

In this assignment, we will implement one "built in" command: `exit`. There is also an optional `dragon` command for extra credit.

Future assignments will generally follow the pattern of other popular shells and implement some of the common builtin commands.

# What else does the shell do?

Beyond implementing built-in commands, a shell acts as a robust broker of input and output. Shells must provide a lot of "glue" to handle streams; for example this is a common sequence you might see in a linux shell: 

```sh
run-some-command | grep "keyword"
```

The `|` symbol is called a pipe, and it serves to stream the output of the first command (`run-some-command`) into the input of the second command (`grep`). We'll implement much of this "glue" logic in future assignments in this course.

# Assignment Details

In this assignment you will implement the first part of `dsh`: a command line parser to interpret commands and their arguments.

You will not implement any command logic, other than exiting when the `exit` command is provided. There is also one optional extra credit for implementing the `dragon` command.

### Step 1 - Review [./starter/dshlib.h](./starter/dshlib.h)

The file [./starter/dshlib.h](./starter/dshlib.h) contains some useful definitions and types. Review the available resources in this file before you start coding - these are intended to make your work easier and more robust!

### Step 2 - Implement [./starter/dsh_cli.c](./starter/dsh_cli.c)

This contains the entrypoint of your shell. Detailed comments are provided to implement `main`.

Shells usually run in a forever loop until a command is issued to exit the shell (usually `exit`); an example is provided in comments to get you started:

```c
    while (1)
    {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }
        // remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // IMPLEMENT THE REST OF THE REQUIREMENTS
    }
```

The libc `fgets` function is a good choice here due to being "lines of input" based. This excerpt from `man 3 fgets` explains why:

> fgets()  reads in at most one less than size characters from stream and stores them into the buffer pointed to by s.  Reading stops after an EOF or a newline. If a newline is read, it is stored into the buffer.  A terminating null byte ('\0') is stored after the last character in the buffer.

Key point, `Reading stops after an EOF or a newline` - which makes it essentially a "line by line" processor. This is an important detail for step 3!

### Step 2a - Implement the built-in function `exit`

Inside the main loop in `main()` you can check for and implement the logic for the `exit` command. When this command is issued, your process should exit with a `0` exit code.

This is also similar to how you would implement the extra credit - instead of existing, you would print more output to STDOUT.

### Step 3 - Implement [./starter/dshlib.c](./starter/dshlib.c)

In this file you need to complete the implementation for `build_cmd_list`:

```c
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    printf(M_NOT_IMPL);
    return EXIT_NOT_IMPL;
}
```

This takes two parameters:

* cmd_line - one complete "line" of user input from the shell; remember `dsh_cli.c` should use EOF / newline (i.e. `fgets()`) to read one line at a time
* clist - pointer to a `command_list_t`; you must populate this structure with parsed commands

Remember - the goal of this assignment is not to implement command logic, but to parse the input string and populate `clist` with parsed commands and their arguments. The comments provide details on logic and references to some helpful definitions.

### Step 4 - Answer Questions

Answer the questions located in [./questions.md](./questions.md).

### Sample Run with Sample Output
The below shows a sample run executing multiple commands and the expected program output:

```bash
➜  solution git:(main) ✗ ./dsh
dsh> cmd
PARSED COMMAND LINE - TOTAL COMMANDS 1
<1> cmd
dsh> cmd_args a1 a2 -a3 --a4
PARSED COMMAND LINE - TOTAL COMMANDS 1
<1> cmd_args [a1 a2 -a3 --a4]
dsh> dragon
[DRAGON for extra credit would print here]
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
➜  solution git:(main) ✗
```

### Extra Credit: +5

Add logic to detect the command "`dragon`" and print the Drexel dragon in ascii art.

Drexel dragon in ascii with spaces preserved:

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

### Extra Credit: ++5

Implement the extra credit for the `/dragon` using compressed/binary data in your code to represent the dragon (i.e. not just using strings/defines).

#### Grading Rubric

This assignment will be weighted 50 points.

- 25 points:  Correct implementation of required functionality
- 5 points:  Code quality (how easy is your solution to follow)
- 15 points: Answering the written questions: [questions.md](./questions.md)
- 5 points:  [EXTRA CREDIT] Implementation of the `dragon` command
- 5 points:  [EXTRA CREDIT++] Implementation of the `dragon` command where the representation of the dragon is compressed in your code

Total points achievable is 60/50. 

#### Automated Testing

In order to help you out the starter package provides a testing script called `test.sh`.  You should expect your tests to fail when running the starter and then start to pass as you implement the required functionality.  Also note a few of the test cases are commented out.


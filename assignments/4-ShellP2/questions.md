1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  We use fork() and execvp() instead of calling execvp() directly to allow the shell to create a new child process for executing commands while the shell continues running. The fork() creates a separate process, and execvp() is then used to replace the child’s image with the new command. This separation ensures that the shell remains responsive, handling multiple commands or processes without being blocked by a single execution

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If the fork() system call fails, it returns a negative value, indicating that the system was unable to create a new child process. In our implementation, we handle this by printing an error message using perror("fork") and then outputting a custom message to inform the user that the process creation failed. This prevents the shell from crashing and allows it to continue accepting and processing other commands

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**: execvp() finds the command to execute by searching through directories listed in the PATH environment variable. The PATH variable contains a colon-separated list of directories where executable programs are located. execvp() iterates through these directories to locate the command and then executes it if found

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  The purpose of calling wait() in the parent process is to ensure that the parent process waits for the child process to finish execution before continuing. This helps to avoid creating zombie processes, which occur when child processes complete but their parent processes don't collect their exit status. Without calling wait(), the parent might continue executing and could leave child processes lingering in the system

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  It is used to retrieve the exit status of a child process that has finished executing. It extracts the return code of the child process, which is set by the program when it terminates, typically indicating whether the process succeeded (return value 0) or failed (non-zero return value). This is important because it allows the parent process to know how the child terminated and handle errors or successes accordingly
6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  In the implementation of build_cmd_buff(), quoted arguments are handled by toggling a flag (in_quotes) whenever a " character is encountered. This ensures that the contents within the quotes are treated as a single argument, even if they contain spaces. This is necessary to correctly parse arguments that include spaces, such as filenames or phrases, without incorrectly splitting them into multiple arguments

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  In this assignment, I made changes to the parsing logic by explicitly handling quoted arguments and ensuring that they are not split at spaces. I also introduced a new method to clear and free memory for each command buffer. One unexpected challenge was ensuring that the buffer size was managed properly, especially when dealing with dynamic memory allocation, which required careful handling to prevent memory leaks or buffer overflow issues. Additionally, adjusting the logic to handle different edge cases like empty input and multiple pipes added complexity to the parsing process

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals in Linux are a form of interprocess communication (IPC) used to notify processes about events or conditions, like user inputs or system errors. They differ from other IPC mechanisms (e.g., pipes, message queues) in that they are primarily for control purposes, not data exchange. Unlike complex IPC methods, signals are efficient for simple notifications but are not suitable for transferring large amounts of data or process synchronization

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**: 
        SIGKILL (Signal 9):
            Use case: This signal forces a process to terminate immediately. It cannot be caught or ignored, making it useful for forcibly stopping processes that are unresponsive or stuck.
        SIGTERM (Signal 15):
            Use case: This is the default signal sent by commands like kill to request a process to terminate. Unlike SIGKILL, SIGTERM can be caught and handled by the process, allowing it to clean up resources before exiting.
        SIGINT (Signal 2):
            Use case: This signal is typically sent by pressing Ctrl+C in the terminal to interrupt a running process. It requests the process to stop what it's doing, but the process can handle it and decide whether to terminate or continue running.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  When a process receives SIGSTOP, it is suspended or paused by the operating system. Unlike SIGINT or SIGTERM, SIGSTOP cannot be caught, ignored, or handled by the process. This is because it is a special signal designed to stop the process immediately, and the operating system ensures that the process cannot override this action, making it impossible for the process to continue without receiving another signal, like SIGCONT, to resume execution

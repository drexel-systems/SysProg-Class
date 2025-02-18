1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  If we didn't use fork, the shell program would exit when execvp is called.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If fork() fails, the exec_cmd returns an error value.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  It uses the PATH environment variable and looks for a file in those directories with the name given as the first argument to execvp.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  It allows the parent process to stop running until the child process is finished. Otherwise the shell would continue running, printing a prompt and waiting for input, even while the command called in execvp is still running.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  It gives the exit status of the child process, which is important if we want to know whether the child process ran without an error.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  It checks for quotation marks, and if one is found, it treats a space character like a regular character until it reaches the end quote. This allows for arguments with strings that include spaces to be passed to execvp.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  It was a bit challenging to figure out how to deal with the space characters within a quote.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  It allows for an external process to send something to another running process to tell it what to do. This is different from just sending input and output text between running programs since a signal can happen at any time.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL is used to terminate a process immediately. SIGTERM is used to request a process terminates, and gives it a chance to do things like save data first. SIGINT is used when control-c is sent from the terminal, and the program can stop if needed.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  SIGSTOP is used to pause a process instead of ending it. It cannot be caught or ignored like SIGINT so that there is a way to pause execution of malicious code.

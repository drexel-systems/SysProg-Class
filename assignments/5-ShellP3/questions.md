1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

It keeps track of all processes in the pids array, and the end of the execute_pipeline function has a loop that calls waitpid on each process in the pids array. This way the parent process has to wait on each child process and cannot continue to the end of the function and return to exec_local_cmd_loop until all child processes are done. If this did not happen, the shell would start printing the prompt and taking user input while the previous commands were still running and printing output to the screen at the same time that the user is typing a new command.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

If pipes were left open, there could be bugs introduced when trying to get data input or output data, since these leftover file descriptors could be inherited by future processes, causing unintended consequences.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

If the directory were changed in an external process, that would not have an effect on the parent process, which would mean the parent process would still be in the original directory. This is not the behavior we want, since we're trying to change the directory of the shell that the user sees, which is the parent process.s

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

I would add another cmd_buff_t* pointer to the command_list struct, and use malloc to allocate space for more commands only if more than 8 are needed. This would have to be freed after all of the commands are executed. I would use malloc and realloc to add only 8 at a time, adding another 8 whenever the total allocated commands is reached. This way I would not be having to allocate memory for each command that is added, but also not using too much memory by allocating a huge buffer that may never be used.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

#include "dshlib.h"

//===================================================================
// HELPER FUNCTIONS - Memory Management (PROVIDED)
//===================================================================

/**
 * alloc_cmd_buff - Allocate memory for cmd_buff internal buffer
 * 
 * This function is provided for you.
 */
int alloc_cmd_buff(cmd_buff_t *cmd_buff)
{
    cmd_buff->_cmd_buffer = malloc(SH_CMD_MAX);
    if (cmd_buff->_cmd_buffer == NULL) {
        return ERR_MEMORY;
    }
    cmd_buff->argc = 0;
    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    }
    return OK;
}

/**
 * free_cmd_buff - Free cmd_buff internal buffer
 * 
 * This function is provided for you.
 */
int free_cmd_buff(cmd_buff_t *cmd_buff)
{
    if (cmd_buff->_cmd_buffer != NULL) {
        free(cmd_buff->_cmd_buffer);
        cmd_buff->_cmd_buffer = NULL;
    }
    cmd_buff->argc = 0;
    return OK;
}

/**
 * clear_cmd_buff - Reset cmd_buff without freeing memory
 * 
 * This function is provided for you.
 */
int clear_cmd_buff(cmd_buff_t *cmd_buff)
{
    cmd_buff->argc = 0;
    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    }
    return OK;
}

//===================================================================
// PARSING FUNCTIONS - COPY FROM PART 1
//===================================================================

/**
 * build_cmd_buff - Parse a single command string into cmd_buff_t
 * 
 * COPY THIS FROM YOUR PART 1 IMPLEMENTATION!
 * 
 * You already implemented this in Part 1. Copy it here and reuse it.
 * 
 * This function takes a single command string and parses it into
 * argc/argv format, handling quotes correctly.
 * 
 * Requirements:
 *   1. Copy cmd_line into cmd_buff->_cmd_buffer
 *   2. Trim leading/trailing whitespace
 *   3. Parse into tokens, handling:
 *      - Regular space-separated tokens
 *      - Quoted strings (preserve spaces inside quotes)
 *      - Remove quotes but keep their contents
 *   4. Store each token pointer in cmd_buff->argv[]
 *   5. Set cmd_buff->argc to number of tokens
 *   6. Ensure cmd_buff->argv[argc] is NULL (REQUIRED for execvp!)
 * 
 * Examples:
 * 
 *   Input:  "ls -la /tmp"
 *   Output: argc=3, argv=["ls", "-la", "/tmp", NULL]
 * 
 *   Input:  "echo \"hello world\""
 *   Output: argc=2, argv=["echo", "hello world", NULL]
 *   Note: Quotes removed, spaces inside preserved
 * 
 *   Input:  "ls    -la"
 *   Output: argc=2, argv=["ls", "-la", NULL]
 *   Note: Multiple spaces collapsed
 * 
 * Hints:
 *   - Use strcpy() to copy to _cmd_buffer
 *   - Modify _cmd_buffer in place (replace spaces with \0)
 *   - argv[] points into _cmd_buffer (don't duplicate strings)
 *   - Handle both " and ' quotes
 * 
 * @param cmd_line: Command string to parse
 * @param cmd_buff: Allocated cmd_buff_t to populate
 * @return: OK on success, WARN_NO_CMDS if empty, error code on failure
 */
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
    // TODO: Copy your implementation from Part 1
    // This is the same function you wrote last week!
    
    return ERR_MEMORY;  // Placeholder - replace with your implementation
}

//===================================================================
// BUILT-IN COMMAND FUNCTIONS
//===================================================================

/**
 * match_command - Check if input is a built-in command
 * 
 * This function is provided for you.
 */
Built_In_Cmds match_command(const char *input)
{
    if (strcmp(input, EXIT_CMD) == 0) {
        return BI_CMD_EXIT;
    }
    if (strcmp(input, DRAGON_CMD) == 0) {
        return BI_CMD_DRAGON;
    }
    if (strcmp(input, CD_CMD) == 0) {
        return BI_CMD_CD;
    }
    if (strcmp(input, RC_CMD) == 0) {
        return BI_CMD_RC;
    }
    return BI_NOT_BI;
}

/**
 * exec_built_in_cmd - Execute built-in commands
 * 
 * YOU NEED TO IMPLEMENT THE CD COMMAND HERE!
 * 
 * Built-in commands run in the shell process itself, not in a child.
 * This is why cd must be built-in - it modifies the shell's own directory.
 * 
 * For cd command:
 *   - If no arguments (argc == 1): do nothing
 *   - If one argument (argc == 2): chdir() to that directory
 *   - Print error if chdir() fails
 * 
 * Example:
 *   cmd->argv = ["cd", "/tmp", NULL]
 *   cmd->argc = 2
 *   Action: chdir("/tmp")
 * 
 * Example (no args):
 *   cmd->argv = ["cd", NULL]
 *   cmd->argc = 1
 *   Action: do nothing (don't change directory)
 * 
 * System call:
 *   int chdir(const char *path);
 *   Returns 0 on success, -1 on error
 * 
 * Error handling:
 *   if (chdir(path) != 0) {
 *       perror("cd");
 *   }
 * 
 * @param cmd: Parsed command buffer
 * @return: BI_CMD_EXIT if exit, BI_EXECUTED if built-in executed, BI_NOT_BI if not built-in
 */
Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd)
{
    Built_In_Cmds bi_cmd = match_command(cmd->argv[0]);
    
    switch (bi_cmd) {
        case BI_CMD_EXIT:
            // Exit is handled in main loop
            return BI_CMD_EXIT;
            
        case BI_CMD_DRAGON:
            // Extra credit from Part 1
            #ifdef DRAGON_IMPLEMENTED
            print_dragon();
            #else
            printf("Dragon not implemented\n");
            #endif
            return BI_EXECUTED;
            
        case BI_CMD_CD:
            // TODO: Implement cd command here
            // Requirements:
            //   1. If argc == 1: do nothing (no directory specified)
            //   2. If argc >= 2: call chdir(cmd->argv[1])
            //   3. If chdir fails: call perror("cd")
            //   4. Return BI_EXECUTED
            
            printf("cd not implemented yet!\n");
            return BI_EXECUTED;
            
        case BI_CMD_RC:
            // Extra credit - print last return code
            // TODO: Implement if doing extra credit
            printf("rc not implemented yet!\n");
            return BI_EXECUTED;
            
        default:
            // Not a built-in command
            return BI_NOT_BI;
    }
}

//===================================================================
// EXECUTION FUNCTIONS - YOU IMPLEMENT THESE
//===================================================================

/**
 * exec_cmd - Execute external command using fork/exec
 * 
 * YOU NEED TO IMPLEMENT THIS FUNCTION! This is the core of Part 2.
 * 
 * This function implements the fork/exec pattern to run external commands.
 * 
 * Algorithm:
 *   1. Fork the process
 *      pid_t pid = fork();
 *      
 *   2. Check fork return value:
 *      if (pid < 0) {
 *          // Fork failed
 *          perror("fork");
 *          return ERR_EXEC_CMD;
 *      }
 *      
 *   3. In child process (pid == 0):
 *      execvp(cmd->argv[0], cmd->argv);
 *      // If we get here, execvp failed
 *      perror("execvp");
 *      exit(EXIT_FAILURE);
 *      
 *   4. In parent process (pid > 0):
 *      int status;
 *      waitpid(pid, &status, 0);
 *      // Extract exit code with WEXITSTATUS(status)
 *      if (WIFEXITED(status)) {
 *          return WEXITSTATUS(status);
 *      }
 *      return ERR_EXEC_CMD;
 * 
 * Important notes:
 *   - execvp() searches PATH environment variable
 *   - cmd->argv MUST be NULL-terminated (argv[argc] == NULL)
 *   - If execvp() returns, it failed (command not found, etc.)
 *   - Child MUST exit() after failed execvp()
 *   - Parent MUST wait for child to avoid zombies
 *   - Use WIFEXITED() to check if child exited normally
 *   - Use WEXITSTATUS() to extract exit code
 * 
 * System calls you'll use:
 *   - fork() - Create child process
 *   - execvp() - Execute command (searches PATH)
 *   - waitpid() - Wait for specific child
 *   - exit() - Terminate child (only if execvp fails)
 * 
 * Macros you'll use:
 *   - WIFEXITED(status) - True if child exited normally
 *   - WEXITSTATUS(status) - Extract exit code from status
 * 
 * For extra credit:
 *   - Check errno after execvp failure
 *   - If errno == ENOENT: command not found (exit 127)
 *   - If errno == EACCES: permission denied (exit 126)
 *   - Print appropriate error messages
 *   - Store return code in global variable
 * 
 * @param cmd: Command to execute
 * @return: Child's exit code on success, error code on failure
 */
int exec_cmd(cmd_buff_t *cmd)
{
    // TODO: Implement fork/exec here
    // This is the main function you need to implement for Part 2
    
    // Suggested approach:
    // 1. Call fork()
    // 2. Check fork return:
    //    - if (pid < 0): fork failed, return error
    //    - if (pid == 0): child process
    //      * Call execvp(cmd->argv[0], cmd->argv)
    //      * If it returns, print error and exit(EXIT_FAILURE)
    //    - if (pid > 0): parent process
    //      * Declare: int status;
    //      * Call waitpid(pid, &status, 0)
    //      * Check WIFEXITED(status)
    //      * Return WEXITSTATUS(status)
    
    printf("exec_cmd not implemented yet!\n");
    return ERR_EXEC_CMD;
}

//===================================================================
// MAIN SHELL LOOP - YOU IMPLEMENT THIS
//===================================================================

/**
 * exec_local_cmd_loop - Main shell loop for Part 2
 * 
 * YOU NEED TO IMPLEMENT THIS FUNCTION!
 * 
 * This is your shell's main loop. It reads commands, parses them,
 * and executes them using fork/exec or as built-ins.
 * 
 * Algorithm:
 *   1. Allocate a cmd_buff_t
 *   2. Loop forever:
 *      a. Print prompt ("dsh2> ")
 *      b. Read line with fgets()
 *      c. Remove trailing newline
 *      d. Check for exit command
 *      e. Parse line into cmd_buff_t
 *      f. Check if built-in command
 *      g. If not built-in, execute with fork/exec
 *      h. Continue loop
 *   3. Free cmd_buff_t when done
 * 
 * Starter code:
 * 
 *   char cmd_line[SH_CMD_MAX];
 *   cmd_buff_t cmd;
 *   int rc;
 *   
 *   // Allocate cmd_buff
 *   if (alloc_cmd_buff(&cmd) != OK) {
 *       return ERR_MEMORY;
 *   }
 *   
 *   while (1) {
 *       // Print prompt
 *       printf("%s", SH_PROMPT);
 *       
 *       // Read input
 *       if (fgets(cmd_line, SH_CMD_MAX, stdin) == NULL) {
 *           printf("\n");
 *           break;
 *       }
 *       
 *       // Remove trailing newline
 *       cmd_line[strcspn(cmd_line, "\n")] = '\0';
 *       
 *       // Check for exit command
 *       if (strcmp(cmd_line, EXIT_CMD) == 0) {
 *           printf("exiting...\n");
 *           break;
 *       }
 *       
 *       // TODO: Parse command line
 *       rc = build_cmd_buff(cmd_line, &cmd);
 *       if (rc != OK || cmd.argc == 0) {
 *           continue; // Empty or parse error
 *       }
 *       
 *       // TODO: Check if built-in command
 *       Built_In_Cmds bi = exec_built_in_cmd(&cmd);
 *       if (bi == BI_CMD_EXIT) {
 *           printf("exiting...\n");
 *           break;
 *       }
 *       if (bi == BI_EXECUTED) {
 *           continue; // Built-in was executed
 *       }
 *       
 *       // TODO: Not built-in, execute with fork/exec
 *       rc = exec_cmd(&cmd);
 *       
 *       // TODO: For extra credit, store rc in global variable
 *   }
 *   
 *   free_cmd_buff(&cmd);
 *   return OK;
 * 
 * Key differences from Part 1:
 *   - Use single cmd_buff_t (not command_list_t)
 *   - Actually execute commands (don't just print)
 *   - Handle built-in commands
 *   - No pipe handling (single commands only)
 * 
 * Error handling:
 *   - Empty input: just continue loop
 *   - Parse failure: continue loop
 *   - Exec failure: command already printed error, continue
 * 
 * @return: OK on success
 */
int exec_local_cmd_loop()
{
    // TODO: Implement this function
    // See detailed comments above for guidance
    
    // Remember:
    // 1. Allocate cmd_buff_t with alloc_cmd_buff()
    // 2. Loop forever (while(1))
    // 3. Read input, parse, execute
    // 4. Handle built-ins separately from external commands
    // 5. Free cmd_buff_t before returning
    
    return OK;
}
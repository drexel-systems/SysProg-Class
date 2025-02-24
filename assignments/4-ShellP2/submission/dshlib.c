#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"
int input_parse(char *cmd_buff);

#include <errno.h> 
/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */

 int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    if (!cmd_buff) return ERR_MEMORY;

    cmd_buff->_cmd_buffer = (char *)malloc(SH_CMD_MAX);
    if (!cmd_buff->_cmd_buffer) return ERR_MEMORY;

    memset(cmd_buff->_cmd_buffer, 0, SH_CMD_MAX);
    cmd_buff->argc = 0;
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv));
    return OK;
}


//Frees dynamically allocated memory inside cmd_buff_t.
int free_cmd_buff(cmd_buff_t *cmd_buff) {
    if (!cmd_buff) return ERR_MEMORY;
    free(cmd_buff->_cmd_buffer);
    cmd_buff->_cmd_buffer = NULL;
    return OK;
}


//Clears cmd_buff_t but does not free memory.
int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->argc = 0;
    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    }
    return OK;
}



//Parses user input into cmd_buff_t.
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    while (isspace((unsigned char)*cmd_line)) cmd_line++;

    if (*cmd_line == '\0') {
        return WARN_NO_CMDS;
    }

    strcpy(cmd_buff->_cmd_buffer, cmd_line);

    cmd_buff->argc = 0;
    char *token = cmd_buff->_cmd_buffer;
    bool in_quotes = false;
    char *start = NULL;

    while (*token != '\0' && cmd_buff->argc < CMD_ARGV_MAX) { 
        if (*token == '"') {
            in_quotes = !in_quotes;
            if (in_quotes) {
                start = token + 1;
            } else {
                *token = '\0';
                cmd_buff->argv[cmd_buff->argc++] = start;
                start = NULL;
            }
        } else if (!in_quotes && isspace((unsigned char)*token)) {
            if (start) {
                *token = '\0';
                cmd_buff->argv[cmd_buff->argc++] = start;
                start = NULL;
            }
        } else if (!start) {
            start = token;
        }
        token++;
    }

    // Add the last argument if there is one
    if (start && *start != '\0' && cmd_buff->argc < CMD_ARGV_MAX) { 
        cmd_buff->argv[cmd_buff->argc++] = start;
    }

    cmd_buff->argv[cmd_buff->argc] = NULL;

    return OK;
}





//Matches a command to a built-in function.
Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, EXIT_CMD) == 0) return BI_CMD_EXIT;
    if (strcmp(input, "cd") == 0) return BI_CMD_CD;
    if (strcmp(input, "dragon") == 0) return BI_CMD_DRAGON;
    return BI_NOT_BI;
}

static int last_return_code = 0;  // Store the last return code

//Executes built-in commands.
Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    if (cmd->argc == 0) return BI_NOT_BI;

    if (strcmp(cmd->argv[0], "dragon") == 0) {
        print_dragon();  // Call the function defined in dragon.c
        return BI_EXECUTED;
    }

    if (strcmp(cmd->argv[0], "rc") == 0) {
        // Print the last return code
        printf("%d\n", last_return_code);
        return BI_EXECUTED;
    }

    Built_In_Cmds type = match_command(cmd->argv[0]);
    
    switch (type){
        case BI_CMD_EXIT:
            return BI_CMD_EXIT;
        
        case BI_CMD_CD:
            if (cmd->argc > 1){
                if (chdir(cmd->argv[1]) != 0){
                    perror("cd");
                }
            }
            return BI_EXECUTED;

        default:
            return BI_NOT_BI;
    }
}



//Executes a command using fork and execvp.
int exec_cmd(cmd_buff_t *cmd) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return ERR_EXEC_CMD;
    }

    if (pid == 0) {
        // Child process: attempt to execute the command
        if (execvp(cmd->argv[0], cmd->argv) == -1) {
            if (errno == ENOENT) {
                fprintf(stderr, "Command not found: %s\n", cmd->argv[0]);
                _exit(127); 
            } else if (errno == EACCES) {
                fprintf(stderr, "Permission denied: %s\n", cmd->argv[0]);
                _exit(126);  
            } else {
                fprintf(stderr, "Error executing command: %s\n", strerror(errno));
                _exit(errno); 
            }
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            last_return_code = WEXITSTATUS(status);
        } else {
            last_return_code = -1;
        }
    }
    return OK;
}


int input_parse(char *cmd_buff) {
    if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
        printf("\n");
        return -1;
    }
    cmd_buff[strcspn(cmd_buff, "\n")] = '\0';
    return 0;
}

int exec_local_cmd_loop() {
    char *cmd_buff;
    int rc = 0;
    cmd_buff_t cmd;

    cmd_buff = malloc(SH_CMD_MAX * sizeof(char));
    if (cmd_buff == NULL) {
        return ERR_MEMORY;
    }

    rc = alloc_cmd_buff(&cmd);
    if (rc != OK) {
        free(cmd_buff);
        return rc;
    }

    while (1) {
        printf("%s", SH_PROMPT);

        // Use input_parse to read and process the input
        if (input_parse(cmd_buff) == -1) {
            break;
        }

        rc = build_cmd_buff(cmd_buff, &cmd);
        if (rc == WARN_NO_CMDS) {
            printf("%s", CMD_WARN_NO_CMD);
            continue;
        } else if (rc == ERR_TOO_MANY_COMMANDS) {
            printf("%s", CMD_ERR_PIPE_LIMIT);
            continue;
        }

        Built_In_Cmds bi_rc = exec_built_in_cmd(&cmd);
        if (bi_rc == BI_CMD_EXIT) {
            rc = OK_EXIT;
            break;
        } else if (bi_rc == BI_EXECUTED) {
            continue;
        }

        rc = exec_cmd(&cmd);
        if (rc != OK) {
            printf("Failed executing command\n");
        }

        clear_cmd_buff(&cmd);
    }

    // Clean up memory
    free(cmd_buff);
    free_cmd_buff(&cmd);

    return rc;
}

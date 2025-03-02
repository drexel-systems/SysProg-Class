#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

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
int exec_local_cmd_loop()
{
    char *cmd_input;
    int rc = 0;
    cmd_buff_t cmd_buff;
    Built_In_Cmds bi_cmd;
    
    // IMPLEMENT MAIN LOOP
    cmd_input = malloc(SH_CMD_MAX);
    if (cmd_input == NULL) {
        perror("malloc:");
        return ERR_MEMORY;
    }
    memset(cmd_input, 0, SH_CMD_MAX);
    
    while(1){
        clear_cmd_buff(&cmd_buff);
        printf("%s", SH_PROMPT);
        if (fgets(cmd_input, ARG_MAX, stdin) == NULL){
            printf("\n");
            break;
        }
        //remove the trailing \n from cmd_buff
        cmd_input[strcspn(cmd_input,"\n")] = '\0';
        
        // IMPLEMENT parsing input to cmd_buff_t *cmd_buff
        rc = build_cmd_buff(cmd_input, &cmd_buff);
        if (rc == WARN_NO_CMDS) {
            printf(CMD_WARN_NO_CMD);
            continue;
        } else if (rc == ERR_TOO_MANY_COMMANDS) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            continue;
        }
        
        // IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
        // the cd command should chdir to the provided directory; if no directory is provided, do nothing
        bi_cmd = exec_built_in_cmd(&cmd_buff);
        if (bi_cmd == BI_CMD_EXIT) {
            return OK;
        } else if (bi_cmd == BI_EXECUTED) {
        } else if (bi_cmd == BI_NOT_BI) {
            // IMPLEMENT if not built-in command, fork/exec as an external command
            rc = exec_cmd(&cmd_buff);
            if (rc == ERR_EXEC_CMD) {
                printf("Error executing command %s\n", cmd_buff.argv[0]);
            }
        }
    }
    
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"
    
    free(cmd_input);
    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
    char *p;
    int i;
    int start_token = 0;
    int start_quote = 0;
    
    if (cmd_line[0] == '\0') {
        return WARN_NO_CMDS;
    }
    
    for (i = 0, p = cmd_line; *p != '\0'; p++) {
        if (*p == '"') {
            if (start_quote) {
                start_quote = 0;
                start_token = 0;
            } else {
                start_quote = 1;
            }
            *p = '\0';
        } else if (*p == SPACE_CHAR && !start_quote) {
            start_token = 0;
            *p = '\0';
        } else if (!start_token) {
            start_token = 1;
            cmd_buff->argv[i] = p;
            i++;
        }
    }
    cmd_buff->argc = i;
    cmd_buff->_cmd_buffer = cmd_line;
    
    return OK;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd)
{
    Built_In_Cmds bi_cmd;
    bi_cmd = match_command(cmd->argv[0]);
    int rc;
    
    switch (bi_cmd) {
        case BI_CMD_EXIT:
        return BI_CMD_EXIT;
        case BI_CMD_CD:
        rc = chdir(cmd->argv[1]);
        if (rc != 0) {
            return BI_CMD_CD;
        }
        return BI_EXECUTED;
        case BI_NOT_BI:
        return BI_NOT_BI;
        case BI_CMD_DRAGON:
        return BI_CMD_DRAGON;
        default:
        fprintf(stderr, "Fatal error");
        exit(EXIT_FAILURE);
    }
}


Built_In_Cmds match_command(const char *input)
{
    if (strcmp(input, EXIT_CMD) == 0) {
        return BI_CMD_EXIT;
    } else if (strcmp(input, "cd") == 0) {
        return BI_CMD_CD;
    } else if (strcmp(input, "dragon") == 0) {
        return BI_CMD_DRAGON;
    } else {
        return BI_NOT_BI;
    }
}

int exec_cmd(cmd_buff_t *cmd)
{
    pid_t f_result, c_result;
    int rc = 0;
    
    f_result = fork();
    if (f_result < 0) {
        return ERR_EXEC_CMD;
    }
    
    if (f_result == 0) {
        rc = execvp(cmd->argv[0], cmd->argv);
        if (rc < 0) {
            exit(EXIT_FAILURE);
        }
    } else {
        wait(&c_result);
        rc = WEXITSTATUS(c_result);
        if (rc != OK) {
            return ERR_EXEC_CMD;
        }
    }
    
    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff)
{
    memset(cmd_buff, 0, sizeof(cmd_buff_t));
    return OK;
}

// int build_cmd_list(char *cmd_line, command_list_t *clist)
// {
//     char* raw_cmds[CMD_MAX] = {0};
//     char* exe;
//     char* arg;
//     int i, j, k;
//     int argsize;

//     memset(clist, 0, sizeof(command_list_t));

//     raw_cmds[clist->num] = strtok(cmd_line, PIPE_STRING);
//     while (raw_cmds[clist->num] != NULL) {
//         if (clist->num == CMD_MAX) {
//             return ERR_TOO_MANY_COMMANDS;
//         }
//         clist->num++;
//         raw_cmds[clist->num] = strtok(NULL, PIPE_STRING);
//     }

//     for (i = 0; i < clist->num; i++) {
//         exe = strtok(raw_cmds[i], " ");

//         if (strlen(exe) >= EXE_MAX || strlen(raw_cmds[i]) - strlen(exe) >= SH_CMD_MAX) {
//             return ERR_CMD_OR_ARGS_TOO_BIG;
//         }
//         strcpy(clist->commands[i].exe, exe);
//         arg = strtok(NULL, " ");
//         k = 0;
//         while (arg != NULL) {
//             if (k != 0) {
//                 clist->commands[i].args[k] = ' ';
//                 k++;
//             }
//             argsize = strlen(arg);
//             for (j = 0; j < argsize; j++) {
//                 clist->commands[i].args[k] = arg[j];
//                 k++;
//             }
//             arg = strtok(NULL, " ");
//         }
//     }
//     return OK;
// }
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
    char *cmd_buff;
    int rc = 0;
    cmd_buff_t cmd;
    Built_In_Cmds bi_cmd;
    
    // TODO IMPLEMENT MAIN LOOP
    cmd_buff = malloc(SH_CMD_MAX);
    memset(cmd_buff, 0, SH_CMD_MAX);
    
    while(1){
        clear_cmd_buff(&cmd);
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
            printf("\n");
            break;
        }
        //remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
        
        // IMPLEMENT parsing input to cmd_buff_t *cmd_buff
        build_cmd_buff(cmd_buff, &cmd);
        
        // IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
        // the cd command should chdir to the provided directory; if no directory is provided, do nothing
        bi_cmd = exec_built_in_cmd(&cmd);
        if (bi_cmd == BI_CMD_EXIT) {
            return OK;
        } else if (bi_cmd == BI_EXECUTED) {
            continue;
        } else if (bi_cmd == BI_NOT_BI) {
            // IMPLEMENT if not built-in command, fork/exec as an external command
            rc = exec_cmd(&cmd);
        }
    }
    
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"
    
    free(cmd_buff);
    return OK;
}

int build_cmd_buff(char *cmd_buff, cmd_buff_t *cmd)
{
    char *p;
    int i;
    int start_token = 0;
    int start_quote = 0;
    
    for (i = 0, p = cmd_buff; *p != '\0'; p++) {
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
            cmd->argv[i] = p;
            i++;
        }
    }
    cmd->argc = i;
    cmd->_cmd_buffer = cmd_buff;
    
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
            return BI_RC;
        }
        return BI_EXECUTED;
    case BI_NOT_BI:
        return BI_NOT_BI;
    default:
        return BI_RC;
    }
}


Built_In_Cmds match_command(const char *input)
{
    if (strcmp(input, EXIT_CMD) == 0) {
        return BI_CMD_EXIT;
    } else if (strcmp(input, "cd") == 0) {
        return BI_CMD_CD;
    } else {
        return BI_NOT_BI;
    }
}

int exec_cmd(cmd_buff_t *cmd)
{
    pid_t f_result, c_result;
    int rc;

    f_result = fork();
    if (f_result < 0) {
        return ERR_EXEC_CMD;
    }

    if (f_result == 0) {
        rc = execvp(cmd->argv[0], cmd->argv);
        if (rc < 0) {
            return ERR_EXEC_CMD;
        }
    } else {
        wait(&c_result);
    }

    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff)
{
    memset(cmd_buff, 0, sizeof(cmd_buff_t));
    return OK;
}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

int alloc_cmd_buff(cmd_buff_t *cmd_buff)
{
    cmd_buff->_cmd_buffer = malloc(SH_CMD_MAX);
    if (cmd_buff->_cmd_buffer == NULL)
    {
        perror("malloc fail");
        return ERR_MEMORY;
    }
    cmd_buff->argc = 0;
    cmd_buff->argv[0] = cmd_buff->_cmd_buffer;
    return OK;
}

int free_cmd_buff(cmd_buff_t *cmd_buff)
{
    if (cmd_buff->_cmd_buffer != NULL)
    {
        free(cmd_buff->_cmd_buffer);
        cmd_buff->_cmd_buffer = NULL;
        cmd_buff->argc = 0;
        memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv));
    }
    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff)
{
    if (cmd_buff->_cmd_buffer == NULL)
        return ERR_MEMORY;

    cmd_buff->argc = 0;
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv));
    memset(cmd_buff->_cmd_buffer, 0, SH_CMD_MAX);
    cmd_buff->argv[0] = cmd_buff->_cmd_buffer;

    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
    char *cl_ptr = cmd_line;
    char *cb_ptr = cmd_buff->_cmd_buffer;
    char curr_char;
    // note students can use ints for true/false booleans
    bool is_space = true;
    bool is_quote = false;

    if (strlen(cmd_line) >= SH_CMD_MAX)
        return ERR_CMD_OR_ARGS_TOO_BIG;

    if (clear_cmd_buff(cmd_buff) != OK)
    {
        return ERR_MEMORY;
    }

    while (*cl_ptr)
    {
        curr_char = *cl_ptr;


        /* extra credit */
        if (curr_char == '<') {
            cl_ptr++; // Move past `<`
            while (*cl_ptr == ' ') cl_ptr++; // Skip spaces
            cmd_buff->input_file = cl_ptr; // Store input filename
            while (*cl_ptr && *cl_ptr != ' ') cl_ptr++; // Move to end of filename
            *cl_ptr = '\0'; // Null terminate the filename
            continue;
        }

        if (*cl_ptr == '>') {
            cl_ptr++; // Move past `>`

            if (*cl_ptr == '>') { // Check for `>>`
                cmd_buff->append_mode = true;
                cl_ptr++; // Move past second `>`
            } else {
                cmd_buff->append_mode = false;
            }

            while (*cl_ptr == ' ') cl_ptr++; // Skip spaces
            cmd_buff->output_file = cl_ptr; // Store output filename
            while (*cl_ptr && *cl_ptr != ' ') cl_ptr++; // Move to end of filename
            *cl_ptr = '\0'; // Null terminate the filename
            continue;
        }
        /* end extra credit */


        // chew up whitespace
        if ((curr_char == ' ') && is_space && !is_quote)
        {
            cl_ptr++;
            continue;
        }

        // hanlde quote ending
        if ((curr_char == '\"') && is_quote)
        {
            is_quote = false;
            is_space = false;
            *cb_ptr = '\0';
            cb_ptr++;
            cl_ptr++;
            continue;
        }

        // handle quote starting
        if ((curr_char == '\"') && !is_quote)
        {
            is_quote = true;
            is_space = true;
            cl_ptr++; // eat up the quote

            if (*cl_ptr)
            { // not at end, starting quote token
                // add token to argv array
                if (cmd_buff->argc < CMD_MAX)
                {
                    *cb_ptr = *cl_ptr;
                    cmd_buff->argv[cmd_buff->argc] = cb_ptr;
                    cmd_buff->argc++;
                }
                else
                {
                    free(cmd_buff->_cmd_buffer);
                    return ERR_CMD_OR_ARGS_TOO_BIG;
                }
            }
            continue;
        }

        // if in quote mode just copy whatever is there
        if (is_quote)
        {
            *cb_ptr = curr_char;
            cb_ptr++;
            cl_ptr++;
            continue;
        }

        // start of a token
        if ((curr_char != ' ') && is_space)
        {
            is_space = false;

            // add token to argv array
            if (cmd_buff->argc < CMD_MAX)
            {
                cmd_buff->argv[cmd_buff->argc] = cb_ptr;
                cmd_buff->argc++;
            }
            else
            {
                free(cmd_buff->_cmd_buffer);
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
        }

        // end of a token
        if ((curr_char == ' ') && !is_space)
        {
            is_space = true;
            *cb_ptr = '\0';
            cb_ptr++;
            cl_ptr++;
            continue;
        }

        // this is just a regular character, copy it
        *cb_ptr = *cl_ptr;
        cb_ptr++;
        cl_ptr++;
    }

    // Now we just have to set the final element in argv[] to a null
    cmd_buff->argv[cmd_buff->argc] = '\0';

    return OK;
}

int free_cmd_list(command_list_t *cmd_lst){
    for (int i = 0; i < cmd_lst->num; i++){
        free_cmd_buff(&(cmd_lst->commands[i]));
    }

    return OK;
}

int build_cmd_list(char *cmd_line, command_list_t *cmd_list){
    char *cmd_token;
    cmd_buff_t *cb;
    int cmd_num = 0;
    int rc;

    if (strlen(cmd_line) == 0)
        return WARN_NO_CMDS;

    //clear the command list
    memset(cmd_list, 0, sizeof(command_list_t));

    //Now lets see how many commands
    cmd_token = strtok(cmd_line, PIPE_STRING);
    while (cmd_token != NULL){
        cb = &(cmd_list->commands[cmd_num]);
        if (alloc_cmd_buff(cb) != OK){
            free_cmd_list(cmd_list);
            return ERR_MEMORY;
        }

        rc = build_cmd_buff(cmd_token, cb);
        switch (rc){
            case ERR_MEMORY:
                free_cmd_list(cmd_list);
                return rc;
            case WARN_NO_CMDS:
                free_cmd_buff(cb);
                printf(CMD_WARN_NO_CMD);
                continue;
            case ERR_CMD_OR_ARGS_TOO_BIG:
                free_cmd_buff(cb);
                printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
                continue;
            default:
                break;
        }

        cmd_num++;
        cmd_token = strtok(NULL, PIPE_STRING); // Get the next command
    }
    cmd_list->num = cmd_num;
    if(cmd_num == 0) //we just had white space
        return WARN_NO_CMDS;

    return OK;
}

Built_In_Cmds match_command(const char *input)
{
    if (strcmp(input, "exit") == 0)
        return BI_CMD_EXIT;
    if (strcmp(input, "dragon") == 0)
        return BI_CMD_DRAGON;
    if (strcmp(input, "cd") == 0)
        return BI_CMD_CD;
    return BI_NOT_BI;
}

extern void print_dragon();

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd)
{
    Built_In_Cmds ctype = BI_NOT_BI;
    ctype = match_command(cmd->argv[0]);

    switch (ctype)
    {
    case BI_CMD_DRAGON:
        print_dragon();
        return BI_EXECUTED;
    case BI_CMD_EXIT:
        return BI_CMD_EXIT;
    case BI_CMD_CD:
        chdir(cmd->argv[1]);
        return BI_EXECUTED;
    default:
        return BI_NOT_BI;
    }
}

int exec_cmd(cmd_buff_t *cmd)
{
    int c_result;
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return ERR_EXEC_CMD;
    }

    if (pid == 0)
    { // Child process
        execvp(cmd->argv[0], cmd->argv);
        exit(1); // Only reached if exec fails
    }

    // wait for child in parent
    wait(&c_result);
    c_result = WEXITSTATUS(c_result); // get exit code

    if (c_result == 0)
        return OK;
    else
        return ERR_EXEC_CMD;
}

int execute_pipeline(command_list_t *clist) {
    int pipes[clist->num - 1][2];  // Array of pipes
    pid_t pids[clist->num];
    int  pids_st[clist->num];         // Array to store process IDs
    Built_In_Cmds bi_cmd;
    int exit_code;

    // Create all necessary pipes
    for (int i = 0; i < clist->num - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Create processes for each command
    for (int i = 0; i < clist->num; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) {  // Child process

            /* extra credit */
            // Handle input redirection
            if (clist->commands[i].input_file) {
                int in_fd = open(clist->commands[i].input_file, O_RDONLY);
                if (in_fd < 0) {
                    perror("open input file");
                    exit(EXIT_FAILURE);
                }
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }

            // Handle output redirection
            if (clist->commands[i].output_file) {
                int flags = O_WRONLY | O_CREAT;
                mode_t mode = 0644;
            
                if (clist->commands[i].append_mode) {
                    flags |= O_APPEND; // Append mode
                } else {
                    flags |= O_TRUNC;  // Truncate mode
                }
            
                int out_fd = open(clist->commands[i].output_file, flags, mode);
                if (out_fd < 0) {
                    perror("open output file");
                    exit(EXIT_FAILURE);
                }
                dup2(out_fd, STDOUT_FILENO);
                close(out_fd);
            }
             /* end extra credit */


            // Set up input pipe for all except first process
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }

            // Set up output pipe for all except last process
            if (i < clist->num - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Close all pipe ends in child
            for (int j = 0; j < clist->num - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            //See if built in
            bi_cmd = exec_built_in_cmd(&(clist->commands[i]));
            if (bi_cmd == BI_CMD_EXIT)
            {
                exit(EXIT_SC);
            }
            if (bi_cmd == BI_EXECUTED)
            {
                exit(0); // done get next command
            }

            // Execute command
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process: close all pipe ends
    for (int i = 0; i < clist->num - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children
    for (int i = 0; i < clist->num; i++) {
        waitpid(pids[i], &pids_st[i], 0);
    }

    //by default get exit code of last process
    //use this as the return value
    exit_code = WEXITSTATUS(pids_st[clist->num - 1]);
    for (int i = 0; i < clist->num; i++) {
        //if any commands in the pipeline are EXIT_SC
        //return that to enable the caller to react
        if (WEXITSTATUS(pids_st[i]) == EXIT_SC)
            exit_code = EXIT_SC;
    }
    return exit_code;
}


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
    command_list_t cmd_list;
    //Built_In_Cmds bi_cmd;

    cmd_buff = malloc(SH_CMD_MAX);
    if (cmd_buff == NULL)
    {
        perror("cant alloc cmd buffer");
        return ERR_MEMORY;
    }

    while (1)
    {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        if (cmd_buff[0] == '\0')
        {
            continue;
        }

        rc = build_cmd_list(cmd_buff, &cmd_list);
        switch (rc)
        {
        case ERR_MEMORY:
            return rc;
        case WARN_NO_CMDS:
            printf(CMD_WARN_NO_CMD);
            continue;
        case ERR_CMD_OR_ARGS_TOO_BIG:
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            continue;
        default:
            break;
        }
        // see if its built in
        

        // we now have an external command to execute here
        rc = execute_pipeline(&cmd_list);
        if (rc == EXIT_SC) {
            printf("exiting...\n");
            break;
        }
    }

    free(cmd_buff);
    free_cmd_list(&cmd_list);
    return OK;
}

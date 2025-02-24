#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"
int split_commands(char *cmd_line, command_list_t *clist);
/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */

 //this is our split command handler
int split_commands(char *cmd_line, command_list_t *clist) {
    char *pipe_saveptr;
    char *command_segment = strtok_r(cmd_line, "|", &pipe_saveptr);

    while (command_segment) {
        // Check command limit
        if (clist->num >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        while (isspace(*command_segment)) command_segment++;

        // Store command segment
        strcpy(clist->commands[clist->num].exe, command_segment);
        clist->num++;

        command_segment = strtok_r(NULL, "|", &pipe_saveptr);
    }

    return OK;
}


int build_cmd_list(char *cmd_line, command_list_t *clist) {
    if (!cmd_line || !clist) {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    memset(clist, 0, sizeof(command_list_t));

    int split_result = split_commands(cmd_line, clist);
    if (split_result != OK) {
        return split_result;
    }

    // Parse each command for arguments
    for (int i = 0; i < clist->num; i++) {
        char *arg_saveptr;
        char *token = strtok_r(clist->commands[i].exe, " \t\n", &arg_saveptr);

        if (!token) {
            continue;
        }

        if (strlen(token) >= EXE_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        strcpy(clist->commands[i].exe, token);

        char args[ARG_MAX] = "";
        token = strtok_r(NULL, " \t\n", &arg_saveptr);

        while (token) {
            size_t curr_len = strlen(args);
            size_t token_len = strlen(token);

            if (curr_len + token_len + 2 >= ARG_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }

            if (curr_len > 0) {
                strcat(args, " ");
            }
            strcat(args, token);
            token = strtok_r(NULL, " \t\n", &arg_saveptr);
        }

        if (strlen(args) > 0) {
            strcpy(clist->commands[i].args, args);
        }
    }

    return (clist->num > 0) ? OK : WARN_NO_CMDS;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

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
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    char* raw_cmds[CMD_MAX] = {0};
    char* exe;
    char* arg;
    int i, j, k;
    int argsize;

    memset(clist, 0, sizeof(command_list_t));

    raw_cmds[clist->num] = strtok(cmd_line, PIPE_STRING);
    while (raw_cmds[clist->num] != NULL) {
        clist->num++;
        raw_cmds[clist->num] = strtok(NULL, PIPE_STRING);
    }

    for (i = 0; i < clist->num; i++) {
        exe = strtok(raw_cmds[i], " ");
        strcpy(clist->commands[i].exe, exe);
        arg = strtok(NULL, " ");
        k = 0;
        while (arg != NULL) {
            if (k != 0) {
                clist->commands[i].args[k] = ' ';
                k++;
            }
            argsize = strlen(arg);
            for (j = 0; j < argsize; j++) {
                clist->commands[i].args[k] = arg[j];
                k++;
            }
            arg = strtok(NULL, " ");
        }
    }
    return OK;
}

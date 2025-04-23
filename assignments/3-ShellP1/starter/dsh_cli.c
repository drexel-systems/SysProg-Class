#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dshlib.h"

/*
 * Implement your main function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.  Since we want fgets to also handle
 * end of file so we can run this headless for testing we need to check
 * the return code of fgets.  I have provided an example below of how
 * to do this assuming you are storing user input inside of the cmd_buff
 * variable.
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
 *
 *   Expected output:
 *
 *      CMD_OK_HEADER      if the command parses properly. You will
 *                         follow this by the command details
 *
 *      CMD_WARN_NO_CMD    if the user entered a blank command
 *      CMD_ERR_PIPE_LIMIT if the user entered too many commands using
 *                         the pipe feature, e.g., cmd1 | cmd2 | ... |
 *
 *  See the provided test cases for output expectations.
 * 
 * sampel run: 
 
 ➜  solution git:(main) ✗ ./dsh
dsh> cmd
PARSED COMMAND LINE - TOTAL COMMANDS 1
<1> cmd
dsh> cmd_args a1 a2 -a3 --a4
PARSED COMMAND LINE - TOTAL COMMANDS 1
<1> cmd_args [a1 a2 -a3 --a4]
dsh> dragon
[DRAGON for extra credit would print here]
dsh> cmd1 | cmd2
PARSED COMMAND LINE - TOTAL COMMANDS 2
<1> cmd1
<2> cmd2
dsh> cmda1 a1 a2 | cmda2 a3 a4 | cmd3 
PARSED COMMAND LINE - TOTAL COMMANDS 3
<1> cmda1 [a1 a2]
<2> cmda2 [a3 a4]
<3> cmd3
dsh> 
warning: no commands provided
dsh> c1 | c2 | c3 | c4 | c5 | c6 | c7 | c8
PARSED COMMAND LINE - TOTAL COMMANDS 8
<1> c1
<2> c2
<3> c3
<4> c4
<5> c5
<6> c6
<7> c7
<8> c8
dsh> c1 | c2 | c3 | c4 | c5 | c6 | c7 | c8 | c9
error: piping limited to 8 commands
dsh> pipe1|pipe2|pipe3 |pipe4             
PARSED COMMAND LINE - TOTAL COMMANDS 4
<1> pipe1
<2> pipe2
<3> pipe3
<4> pipe4
dsh> pipe1|pipe2 |pipe3 pipe4| pipe5
PARSED COMMAND LINE - TOTAL COMMANDS 4
<1> pipe1
<2> pipe2
<3> pipe3 [pipe4]
<4> pipe5
dsh> exit
➜  solution git:(main) ✗
 */
int main()
{
    char *cmd_buff;
    int rc = 0;
    command_list_t clist;

    // TODO allocate memory for cmd_buff

    while(1){
        // TODO print prompt and fgets input

        // TODO remove \n from cmd_buff

        // TODO if exit command, exit return code 0

        // TODO optional extra credit - print dragon if dragon command

        rc = build_cmd_list(cmd_buff, &clist);

        if (rc == OK){
            // TODO print CMD_OK_HEADER with number of commands

            // TODO loop over commands and print command and arguments using formatting in "sample run" above in comments
        }

        if (rc == WARN_NO_CMDS){
            printf(CMD_WARN_NO_CMD);
        }

        if (rc == ERR_TOO_MANY_COMMANDS){
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
        }
    }

    // TODO don't forget to free cmd_buff
}
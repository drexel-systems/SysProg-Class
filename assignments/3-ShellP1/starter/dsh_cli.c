#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include "dshlib.h"
#include "dragon_data.h"

#define CHUNK 16384 // (16 kb buffer)

// must link z library when compiling now
// so gcc dsh_cli.c -lz -o dsh
void print_dragon()
{
	unsigned char out[CHUNK];
	z_stream strm = {0};

	strm.avail_in = dragon_gz_len;
	strm.next_in = dragon_gz;

	if (inflateInit2(&strm, 16 + MAX_WBITS) != Z_OK)
	{
		return;
	}

	int status;
	do
	{
		strm.avail_out = CHUNK;
		strm.next_out = out;
		status = inflate(&strm, Z_NO_FLUSH);
		fwrite(out, 1, CHUNK - strm.avail_out, stdout);
	} while (status != Z_STREAM_END);

	inflateEnd(&strm);
}

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
	char *token;
	char *token_ptr;
	char *end;
	char exec[EXE_MAX];
	char arg[ARG_MAX];

	int cmd_count = 0;
	int arg_count = 0;

	memset(clist, 0, sizeof(command_list_t));

	token = strtok(cmd_line, "|");

	while (token != NULL)
	{
		// trim leading and trailing spaces
		while (*token == ' ')
			token++;
		end = token + strlen(token) - 1;
		while (*end == ' ')
		{
			*end = '\0';
			end--;
		}

		// error handling
		if (strlen(token) == 0)
		{
			return WARN_NO_CMDS;
		}

		// save executable
		int i = 0;
		while (token[i] != ' ' && token[i] != '\0')
		{
			exec[i] = token[i];
			i++;
		}
		exec[i] = '\0';
		strncpy(clist->commands[cmd_count].exe, exec, EXE_MAX);
		i++;

		// extract arguments (anything after the executable)
		int j = 0;
		while (i < strlen(token) && token[i] != '\0')
		{
			arg[j] = token[i];
			i++;
			j++;
		}
		arg[j] = '\0';

		if (strlen(arg) > ARG_MAX)
		{
			return ERR_CMD_OR_ARGS_TOO_BIG;
		}
		strncpy(clist->commands[cmd_count].args, arg, ARG_MAX);

		cmd_count++;
		if (cmd_count > CMD_MAX)
		{
			return ERR_TOO_MANY_COMMANDS;
		}
		token = strtok(NULL, "|");
	}
	if (cmd_count == 0)
	{
		return WARN_NO_CMDS;
	}

	clist->num = cmd_count;
	return OK;
}

int main()
{
	char *cmd_buff = malloc(SH_CMD_MAX * sizeof(char));
	int rc = 0;
	command_list_t clist;

	// printf(M_NOT_IMPL);
	// exit(EXIT_NOT_IMPL);

	while (1)
	{
		printf("%s", SH_PROMPT);
		if (fgets(cmd_buff, ARG_MAX, stdin) == NULL)
		{
			printf("\n");
			break;
		}
		// remove the trailing \n from cmd_buff
		cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

		if (strcmp(cmd_buff, EXIT_CMD) == 0)
		{
			free(cmd_buff);
			exit(OK);
		}
		else if (strcmp(cmd_buff, "dragon") == 0)
		{
			print_dragon();
			continue;
		}

		// parse command
		rc = build_cmd_list(cmd_buff, &clist);
		if (rc == OK)
		{
			printf(CMD_OK_HEADER, clist.num);
			for (int k = 0; k < clist.num; k++)
			{
				command_t i = clist.commands[k];
				printf("<%d> %s", k + 1, i.exe);
				if (strlen(i.args) != 0)
				{
					printf(" [%s]\n", i.args);
				}
				else
				{
					printf("\n");
				}
			}
		}
		else if (rc == WARN_NO_CMDS)
		{
			printf(CMD_WARN_NO_CMD);
		}
		else if (rc == ERR_TOO_MANY_COMMANDS)
		{
			printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
		}
	}
}

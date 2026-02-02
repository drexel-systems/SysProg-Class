#ifndef __DSHLIB_H__
#define __DSHLIB_H__

//===================================================================
// CONSTANTS FOR COMMAND STRUCTURE SIZES
//===================================================================

#define EXE_MAX 64           // Maximum length of command name
#define ARG_MAX 256          // Maximum length of arguments string
#define CMD_MAX 8            // Maximum number of piped commands
#define CMD_ARGV_MAX (CMD_MAX + 1)  // Max args + NULL terminator

// Longest command that can be read from the shell
#define SH_CMD_MAX (EXE_MAX + ARG_MAX)

//===================================================================
// DATA STRUCTURES
//===================================================================

/**
 * cmd_buff_t - Represents a single parsed command
 * 
 * This structure holds one command with its arguments, similar to
 * how execvp() expects arguments.
 * 
 * Fields:
 *   argc        - Number of arguments (including command name)
 *   argv        - Array of argument strings (NULL-terminated)
 *   _cmd_buffer - Internal buffer for string storage
 * 
 * Example: "ls -la /tmp" becomes:
 *   argc = 3
 *   argv[0] = "ls"
 *   argv[1] = "-la"
 *   argv[2] = "/tmp"
 *   argv[3] = NULL  (required for execvp in future assignments)
 * 
 * Memory Management:
 *   - Use alloc_cmd_buff() to allocate
 *   - Use free_cmd_buff() to free
 *   - _cmd_buffer is for internal use, do not modify directly
 */
typedef struct cmd_buff
{
    int  argc;
    char *argv[CMD_ARGV_MAX];
    char *_cmd_buffer;  // Internal buffer - allocated/freed by helper functions
} cmd_buff_t;

/**
 * command_list_t - Contains multiple commands split by pipes
 * 
 * This structure holds all commands from a single input line,
 * separated by pipe characters.
 * 
 * Fields:
 *   num      - Number of commands in the list (1 to CMD_MAX)
 *   commands - Array of cmd_buff_t structures
 * 
 * Example: "ls | grep txt | wc -l" becomes:
 *   num = 3
 *   commands[0] = {argc=1, argv=["ls", NULL]}
 *   commands[1] = {argc=2, argv=["grep", "txt", NULL]}
 *   commands[2] = {argc=2, argv=["wc", "-l", NULL]}
 * 
 * Memory Management:
 *   - Allocate each cmd_buff in commands[] separately
 *   - Use free_cmd_list() to free all memory
 */
typedef struct command_list
{
    int num;
    cmd_buff_t commands[CMD_MAX];
} command_list_t;

//===================================================================
// SPECIAL CHARACTERS
//===================================================================

#define SPACE_CHAR  ' '
#define PIPE_CHAR   '|'
#define PIPE_STRING "|"

//===================================================================
// SHELL CONSTANTS
//===================================================================

#define SH_PROMPT "dsh> "
#define EXIT_CMD  "exit"
#define EXIT_SC   99

//===================================================================
// RETURN CODES
//===================================================================

#define OK                      0   // Success
#define WARN_NO_CMDS           -1   // Empty command line
#define ERR_TOO_MANY_COMMANDS  -2   // More than CMD_MAX commands
#define ERR_CMD_OR_ARGS_TOO_BIG -3  // Command or args exceed buffer
#define ERR_CMD_ARGS_BAD       -4   // Malformed arguments (for extra credit)
#define ERR_MEMORY             -5   // Memory allocation failure
#define ERR_EXEC_CMD           -6   // Command execution failure
#define OK_EXIT                -7   // Exit command received

//===================================================================
// OUTPUT MESSAGE CONSTANTS
//===================================================================

#define CMD_OK_HEADER       "PARSED COMMAND LINE - TOTAL COMMANDS %d\n"
#define CMD_WARN_NO_CMD     "warning: no commands provided\n"
#define CMD_ERR_PIPE_LIMIT  "error: piping limited to %d commands\n"

//===================================================================
// FUNCTION PROTOTYPES - Memory Management
//===================================================================

/**
 * alloc_cmd_buff - Allocate memory for a cmd_buff_t structure
 * 
 * Allocates the internal _cmd_buffer for storing command strings.
 * Must be called before using build_cmd_buff().
 * 
 * @param cmd_buff: Pointer to cmd_buff_t to allocate
 * @return: OK on success, ERR_MEMORY on failure
 */
int alloc_cmd_buff(cmd_buff_t *cmd_buff);

/**
 * free_cmd_buff - Free memory allocated for a cmd_buff_t
 * 
 * Frees the internal _cmd_buffer. Call this when done with a cmd_buff.
 * 
 * @param cmd_buff: Pointer to cmd_buff_t to free
 * @return: OK on success
 */
int free_cmd_buff(cmd_buff_t *cmd_buff);

/**
 * clear_cmd_buff - Reset a cmd_buff_t to initial state
 * 
 * Clears argc and argv without freeing memory. Useful for reuse.
 * 
 * @param cmd_buff: Pointer to cmd_buff_t to clear
 * @return: OK on success
 */
int clear_cmd_buff(cmd_buff_t *cmd_buff);

/**
 * free_cmd_list - Free all memory in a command_list_t
 * 
 * Frees memory for all cmd_buff_t structures in the list.
 * Call this when done with a command_list_t.
 * 
 * @param cmd_lst: Pointer to command_list_t to free
 * @return: OK on success
 */
int free_cmd_list(command_list_t *cmd_lst);

//===================================================================
// FUNCTION PROTOTYPES - Parsing
//===================================================================

/**
 * build_cmd_buff - Parse a single command string into cmd_buff_t
 * 
 * Takes a command string (e.g., "ls -la /tmp") and populates
 * a cmd_buff_t structure with parsed command and arguments.
 * 
 * @param cmd_line: String containing single command (no pipes)
 * @param cmd_buff: Pointer to allocated cmd_buff_t to populate
 * @return: OK on success, error code on failure
 * 
 * Example:
 *   Input:  "ls -la /tmp"
 *   Output: argc=3, argv=["ls", "-la", "/tmp", NULL]
 */
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff);

/**
 * build_cmd_list - Parse command line with pipes into command_list_t
 * 
 * Takes a full command line (possibly with pipes) and splits it
 * into individual commands, storing each in a command_list_t.
 * 
 * This is the main parsing function you need to implement!
 * 
 * @param cmd_line: Full command line string from user input
 * @param clist: Pointer to command_list_t to populate
 * @return: OK on success, error code on failure
 * 
 * Example:
 *   Input:  "ls | grep txt | wc -l"
 *   Output: num=3, commands[0]="ls", commands[1]="grep txt", commands[2]="wc -l"
 * 
 * Error cases:
 *   - Empty/whitespace only input -> WARN_NO_CMDS
 *   - More than CMD_MAX commands -> ERR_TOO_MANY_COMMANDS
 *   - Memory allocation fails -> ERR_MEMORY
 */
int build_cmd_list(char *cmd_line, command_list_t *clist);

//===================================================================
// FUNCTION PROTOTYPES - Built-in Commands
//===================================================================

/**
 * Built-in command types
 */
typedef enum {
    BI_CMD_EXIT,    // exit command
    BI_CMD_DRAGON,  // dragon command (extra credit)
    BI_CMD_CD,      // cd command (future assignment)
    BI_NOT_BI,      // Not a built-in command
    BI_EXECUTED,    // Built-in command was executed
} Built_In_Cmds;

/**
 * match_command - Check if input matches a built-in command
 * 
 * @param input: Command string to check
 * @return: Built_In_Cmds enum value
 */
Built_In_Cmds match_command(const char *input);

/**
 * exec_built_in_cmd - Execute a built-in command
 * 
 * @param cmd: Pointer to cmd_buff_t containing command
 * @return: Built_In_Cmds enum indicating result
 */
Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd);

//===================================================================
// FUNCTION PROTOTYPES - Main Execution
//===================================================================

/**
 * exec_local_cmd_loop - Main shell loop (IMPLEMENT THIS!)
 * 
 * This is the core function you need to implement for Part 1.
 * It should:
 *   1. Loop forever accepting user input
 *   2. Parse input into command_list_t
 *   3. Handle exit command
 *   4. Print parsed commands in required format
 *   5. Free memory before next iteration
 * 
 * @return: OK on normal exit, error code on failure
 */
int exec_local_cmd_loop();

/**
 * exec_cmd - Execute a single command (future assignment)
 * 
 * @param cmd: Pointer to cmd_buff_t to execute
 * @return: Return code from command execution
 */
int exec_cmd(cmd_buff_t *cmd);

/**
 * execute_pipeline - Execute piped commands (future assignment)
 * 
 * @param clist: Pointer to command_list_t containing pipeline
 * @return: Return code from pipeline execution
 */
int execute_pipeline(command_list_t *clist);

#endif // __DSHLIB_H__
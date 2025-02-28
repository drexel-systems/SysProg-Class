#ifndef __RSH_LIB_H__
    #define __RSH_LIB_H__

#include "dshlib.h"

//common remote shell client and server constants and definitions


//Constants for communication
//Note that these should work fine in a local VM but you will likely have
//to change the port number if you are working on tux.
#define RDSH_DEF_PORT           1234        //Default port #
#define RDSH_DEF_SVR_INTFACE    "0.0.0.0"   //Default start all interfaces
#define RDSH_DEF_CLI_CONNECT    "127.0.0.1" //Default server is running on
                                            //localhost 127.0.0.1

//constants for buffer sizes
#define RDSH_COMM_BUFF_SZ       (1024*64)   //64K
#define STOP_SERVER_SC          200         //returned from pipeline excution
                                            //if the command is to stop the
                                            //server.  See documentation for 
                                            //exec_client_requests() for more info

//end of message delimiter.  This is super important.  TCP is a stream, therefore
//the protocol designer is responsible for managing where messages begin and end
//there are many common techniques for this, but one of the simplest ways is to
//use an end of stream marker.  Since rsh is a "shell" program we will be using
//ascii code 0x04, which is commonly used as the end-of-file (EOF) character in
//linux based systems. 
static const char RDSH_EOF_CHAR = 0x04;    

//rdsh specific error codes for functions
#define ERR_RDSH_COMMUNICATION  -50     //Used for communication errors
#define ERR_RDSH_SERVER         -51     //General server errors
#define ERR_RDSH_CLIENT         -52     //General client errors
#define ERR_RDSH_CMD_EXEC       -53     //RSH command execution errors
#define WARN_RDSH_NOT_IMPL      -99     //Not Implemented yet warning

//Output message constants for server
#define CMD_ERR_RDSH_COMM   "rdsh-error: communications error\n"
#define CMD_ERR_RDSH_EXEC   "rdsh-error: command execution error\n"
#define CMD_ERR_RDSH_ITRNL  "rdsh-error: internal server error - %d\n"
#define CMD_ERR_RDSH_SEND   "rdsh-error: partial send.  Sent %d, expected to send %d\n"
#define RCMD_SERVER_EXITED  "server appeared to terminate - exiting\n"

//Output message constants for client
#define RCMD_MSG_CLIENT_EXITED  "client exited: getting next connection...\n"
#define RCMD_MSG_SVR_STOP_REQ   "client requested server to stop, stopping...\n"
#define RCMD_MSG_SVR_EXEC_REQ   "rdsh-exec:  %s\n"
#define RCMD_MSG_SVR_RC_CMD     "rdsh-exec:  rc = %d\n"

//client prototypes for rsh_cli.c - - see documentation for each function to
//see what they do
int start_client(char *address, int port);
int client_cleanup(int cli_socket, char *cmd_buff, char *rsp_buff, int rc);
int exec_remote_cmd_loop(char *address, int port);
    

//server prototypes for rsh_server.c - see documentation for each function to
//see what they do
int start_server(char *ifaces, int port, int is_threaded);
int boot_server(char *ifaces, int port);
int stop_server(int svr_socket);
int send_message_eof(int cli_socket);
int send_message_string(int cli_socket, char *buff);
int process_cli_requests(int svr_socket);
int exec_client_requests(int cli_socket);
int rsh_execute_pipeline(int socket_fd, command_list_t *clist);

// SEE COMMENTS IN THE CODE, THESE ARE OPTIONAL IN CASE YOU WANT TO PROVIDE
// SUPPORT FOR BUILT-IN FUNCTIONS DIFFERENTLY 
Built_In_Cmds rsh_match_command(const char *input);
Built_In_Cmds rsh_built_in_cmd(cmd_buff_t *cmd);

#endif
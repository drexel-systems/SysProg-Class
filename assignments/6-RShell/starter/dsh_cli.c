#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include <getopt.h>

#include "dshlib.h"
#include "rshlib.h"


/*
 * Used to pass startup parameters back to main
 */
#define MODE_LCLI   0       //Local client
#define MODE_SCLI   1       //Socket client
#define MODE_SSVR   2       //Socket server

typedef struct cmd_args{
  int   mode;
  char  ip[16];   //e.g., 192.168.100.101\0
  int   port;
  int   threaded_server;
}cmd_args_t;



//You dont really need to understand this but the C runtime library provides
//an getopt() service to simplify handling command line arguments.  This
//code will help setup dsh to handle triggering client or server mode along
//with passing optional connection parameters. 

void print_usage(const char *progname) {
  printf("Usage: %s [-c | -s] [-i IP] [-p PORT] [-x] [-h]\n", progname);
  printf("  Default is to run %s in local mode\n", progname);
  printf("  -c            Run as client\n");
  printf("  -s            Run as server\n");
  printf("  -i IP         Set IP/Interface address (only valid with -c or -s)\n");
  printf("  -p PORT       Set port number (only valid with -c or -s)\n");
  printf("  -x            Enable threaded mode (only valid with -s)\n");
  printf("  -h            Show this help message\n");
  exit(0);
}

void parse_args(int argc, char *argv[], cmd_args_t *cargs) {
  int opt;
  memset(cargs, 0, sizeof(cmd_args_t));

  //defaults
  cargs->mode = MODE_LCLI;
  cargs->port = RDSH_DEF_PORT;

  while ((opt = getopt(argc, argv, "csi:p:xh")) != -1) {
      switch (opt) {
          case 'c':
              if (cargs->mode != MODE_LCLI) {
                  fprintf(stderr, "Error: Cannot use both -c and -s\n");
                  exit(EXIT_FAILURE);
              }
              cargs->mode = MODE_SCLI;
              strncpy(cargs->ip, RDSH_DEF_CLI_CONNECT, sizeof(cargs->ip) - 1);
              break;
          case 's':
              if (cargs->mode != MODE_LCLI) {
                  fprintf(stderr, "Error: Cannot use both -c and -s\n");
                  exit(EXIT_FAILURE);
              }
              cargs->mode = MODE_SSVR;
              strncpy(cargs->ip, RDSH_DEF_SVR_INTFACE, sizeof(cargs->ip) - 1);
              break;
          case 'i':
              if (cargs->mode == MODE_LCLI) {
                  fprintf(stderr, "Error: -i can only be used with -c or -s\n");
                  exit(EXIT_FAILURE);
              }
              strncpy(cargs->ip, optarg, sizeof(cargs->ip) - 1);
              cargs->ip[sizeof(cargs->ip) - 1] = '\0';  // Ensure null termination
              break;
          case 'p':
              if (cargs->mode == MODE_LCLI) {
                  fprintf(stderr, "Error: -p can only be used with -c or -s\n");
                  exit(EXIT_FAILURE);
              }
              cargs->port = atoi(optarg);
              if (cargs->port <= 0) {
                  fprintf(stderr, "Error: Invalid port number\n");
                  exit(EXIT_FAILURE);
              }
              break;
          case 'x':
              if (cargs->mode != MODE_SSVR) {
                  fprintf(stderr, "Error: -x can only be used with -s\n");
                  exit(EXIT_FAILURE);
              }
              cargs->threaded_server = 1;
              break;
          case 'h':
              print_usage(argv[0]);
              break;
          default:
              print_usage(argv[0]);
      }
  }

  if (cargs->threaded_server && cargs->mode != MODE_SSVR) {
      fprintf(stderr, "Error: -x can only be used with -s\n");
      exit(EXIT_FAILURE);
  }
}



/* DO NOT EDIT
 * main() logic fully implemented to:
 *    1. run locally (no parameters)
 *    2. start the server with the -s option
 *    3. start the client with the -c option
*/
int main(int argc, char *argv[]){
  cmd_args_t cargs;
  int rc;

  memset(&cargs, 0, sizeof(cmd_args_t));
  parse_args(argc, argv, &cargs);

  switch(cargs.mode){
    case MODE_LCLI:
      printf("local mode\n");
      rc = exec_local_cmd_loop();
      break;
    case MODE_SCLI:
      printf("socket client mode:  addr:%s:%d\n", cargs.ip, cargs.port);
      rc = exec_remote_cmd_loop(cargs.ip, cargs.port);
      break;
    case MODE_SSVR:
      printf("socket server mode:  addr:%s:%d\n", cargs.ip, cargs.port);
      if (cargs.threaded_server){
        printf("-> Multi-Threaded Mode\n");
      } else {
        printf("-> Single-Threaded Mode\n");
      }
      rc = start_server(cargs.ip, cargs.port, cargs.threaded_server);
      break;
    default:
      printf("error unknown mode\n");
      exit(EXIT_FAILURE);
  }

  printf("cmd loop returned %d\n", rc);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dshlib.h"

/* DO NOT EDIT
 * main() logic moved to exec_local_cmd_loop() in dshlib.c
*/

// NOTE!!!!!!! Mr. TA, to check my dragon, please use this command!!!!!
// do this: 
// cd bats
// ./dsh 
// dragon

int main(){
  int rc = exec_local_cmd_loop();
  printf("cmd loop returned %d\n", rc);
}
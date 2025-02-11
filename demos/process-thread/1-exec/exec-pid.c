#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

//This program validates that the process started by execv takes over the
//process that called it.  The first thing this program does is print its
//pid.  It then execv() another program that prints its process id.  Notice
//the behavior. 
int main(){
    char *args[] = {"./my-pid", 0};
    int rc;

    printf("exec-pid: My process id is: %d\n", getpid());
    rc = execv(args[0], args);
    printf("Well did we get here?\n");
    printf("execv returned %d\n",rc);
}
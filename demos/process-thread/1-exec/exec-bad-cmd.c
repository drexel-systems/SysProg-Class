#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

//This program demonstrates what the execv system call does
//Lets explore, see below for what is going on....

int main(){
    char *args[] = {"/bin/this_program_does_not_exist",  0};
    int rc;

    rc = execv(args[0], args);
    printf("Well did we get here?\n");
    printf("execv returned %d when trying to run \"%s\"\n",rc,args[0]);
    perror("execv error");
}

//Whats going on?
//The execv system call REPLACES the currently running process with the
//executable passed to the first argument of execv.  So, unless there is
//an error the call from execv() NEVER RETURNS.  Instead, the executable
//passed as the first argument runs and takes over the current process. 
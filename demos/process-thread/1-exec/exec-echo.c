#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

//This program demonstrates what the execv system call does
//Lets explore, see below for what is going on....

int main(){
    char *args[] = {"/bin/echo", "Hi", "Class", 0};
    int rc;

    rc = execv(args[0], args);
    printf("Well did we get here?\n");
    printf("execv returned %d\n",rc);
}

//Whats going on?
//The execv system call REPLACES the currently running process with the
//executable passed to the first argument of execv.  So, unless there is
//an error the call from execv() NEVER RETURNS.  Instead, the executable
//passed as the first argument runs and takes over the current process. 
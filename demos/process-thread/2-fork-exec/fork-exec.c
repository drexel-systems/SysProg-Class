#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

//This pattern is much more common for creating another process
//it involves using both fork and exec

int main(){
    int f_result, c_result;

    f_result = fork();
    if (f_result < 0){
        perror("fork error");
        exit(1);
    }

    if (f_result == 0){
        //The child will now exec, basically shape shifting itself
        char *args[] = {"./my-pid", 0};
        int rc;

        rc = execv(args[0], args);
        if (rc < 0){
            //we need to handle the error to make sure execution does
            //not continue, or likely wierd things will happen. 
            perror("fork error");
            exit(42);   //exit code for bad fork, but we want to stop
                        //here if the fork failed
        }
    } else{
        //This will be where the parent picks up
        //lets print f_result
        printf("[p] Parent process id is %d\n", getpid());
        printf("[p] Child process id is %d\n", f_result);
        printf("[p] Waiting for child to finish...\n\n");
        wait(&c_result);

        //we can use a macro in the runtime library to extract
        //the status code from what wait_returns
        printf("[p] The child exit status is %d\n", WEXITSTATUS(c_result));
        
        exit(22);
    }
}
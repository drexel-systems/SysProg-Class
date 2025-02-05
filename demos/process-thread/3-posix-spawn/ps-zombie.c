#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include <spawn.h>

//pick up the environment from the c runtime library
extern char **environ;

int main(){
    char *args[] = {"./sleeper", "20", 0};
    int rc;
    int result;

    rc = posix_spawn(&result, args[0], NULL, NULL, args, environ);
    if (rc != 0){
        perror("posix spawn");
        exit(1);
    }
    
     //This will be where the parent picks up
        //lets print f_result
        printf("[p] Parent process id is %d\n", getpid());
        printf("[p] Parent's Parent process id is %d\n", getppid());
        printf("[p] Child process id is %d\n", result);
        printf("[p] Waiting for child to finish...\n\n");

        //sleep 10 seconds
        sleep(10);
        //wait(&c_result);

        //we can use a macro in the runtime library to extract
        //the status code from what wait_returns
        printf("[p] The child exit status is %d\n", WEXITSTATUS(result));
        
        exit(22);
}
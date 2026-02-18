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
    //int status;

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


        
        // Use waitpid to wait for the specific child (result)
        // This fills the 'status' variable with the return info
        //if (waitpid(result, &status, 0) == -1) {
        //    perror("waitpid");
        //    exit(1);
        //}
        //if (WIFEXITED(status)) {
        //    printf("[p] The child exit status is %d\n", WEXITSTATUS(status));
        //}


        
        exit(22);
}
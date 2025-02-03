#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include <spawn.h>

//pick up the environment from the c runtime library
extern char **environ;

int main(){
    char *args[] = {"./my-pid", 0};
    int rc;
    int result;

    rc = posix_spawn(&result, args[0], NULL, NULL, args, environ);
    if (rc != 0){
        perror("posix spawn");
        exit(1);
    }
    wait(&result);
    result = (result >> 8) & 0xFF;
    printf("[p] child exited with return code %d\n", result);
}
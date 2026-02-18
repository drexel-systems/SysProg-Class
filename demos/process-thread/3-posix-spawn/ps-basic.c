#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <spawn.h>

// pick up the environment from the c runtime library
extern char **environ;

int main()
{
    char *args[] = {"./my-pid", 0};
    int rc;
    int result;
    int status;

    long tid = syscall(SYS_gettid);

    printf("[p] my-pid: My process id is: %d\n", getpid());
    printf("[p] my-pid: My thread id is: %ld\n", tid);
    printf("[p] my-pid: Note in a mult-threaded program pid != tid\n");

    rc = posix_spawn(&result, args[0], NULL, NULL, args, environ);
    if (rc != 0)
    {
        perror("posix spawn");
        exit(1);
    }

    // Use waitpid to wait for the specific child (result)
    // This fills the 'status' variable with the return info
    if (waitpid(result, &status, 0) == -1) {
        perror("waitpid");
        exit(1);
    }
    if (WIFEXITED(status)) {
        printf("[p] child exited with return code %d\n", WEXITSTATUS(status));
    }
}

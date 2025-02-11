#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// This pattern is often not done it involves a program that can take 2 different
// concurrent paths in two different processes

int child_logic()
{
    printf("[c] Hello I am the child, with pid %d\n\n", getpid());
    return 0;
}

int main()
{
    int f_result, c_result;

    c_result = -1;

    f_result = fork();
    if (f_result < 0)
    {
        perror("fork error");
        exit(1);
    }

    //--------------------------------------------------------------------------------//

    // Discussion:  Fork returns twice here
    if (f_result == 0)
    {
        // The child will get f_result of 0
        child_logic();
        exit(242);
    }
    else
    {
        // This will be where the parent picks up
        // lets print f_result
        printf("[p] The fork() command returned %d\n", f_result);
        printf("[p] My parent process id is %d\n", getpid());
        printf("[p] Waiting for child to finish...\n\n");
        wait(&c_result);

        // we can use a macro in the runtime library to extract
        // the status code from what wait_returns
        printf("[p] The raw c_result value is %d\n", c_result);
        printf("[p] The child exit status is %d\n", WEXITSTATUS(c_result));

        // OR, even better we can understand what is going on.  c_result
        // is a 32 bit number encoded with several peices of information.
        // The status code is in the second byte.  We can extract it with
        // some bit manipulation and shifting....
        c_result = (c_result >> 8) & 0x000000FF;
        printf("[p] The child exited with code (manually extracted) %d\n", c_result);

        exit(22);
    }
}
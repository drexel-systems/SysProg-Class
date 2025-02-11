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

    long tid = syscall(SYS_gettid);

    printf("[c] my-pid: My process id is: %d\n", getpid());
    printf("[c] my-pid: My thread id is: %ld\n", tid);
    printf("[c] my-pid: Note in a mult-threaded program pid != tid\n");

    rc = posix_spawn(&result, args[0], NULL, NULL, args, environ);
    if (rc != 0)
    {
        perror("posix spawn");
        exit(1);
    }
    wait(&result);
    result = (result >> 8) & 0xFF; // get the result without using a macro - same logic as WEXITSTATUS (see comment below)
    printf("[p] child exited with return code %d\n", result);
}

// Extract the second byte from a 32-bit integer
// result = (result >> 8) & 0xFF;
//
// Before shift (example: 0x12345678):
//
//  31       24 23       16 15        8 7         0  (bit positions)
// +--------+--------+--------+--------+
// |   12   |   34   |   56   |   78   |  Original value (0x12345678)
// +--------+--------+--------+--------+
//
// Step 1: Right shift by 8 (>> 8)
//
// +--------+--------+--------+--------+
// |   00   |   12   |   34   |   56   |  Now 56 is in the lower 8 bits
// +--------+--------+--------+--------+
//
// Step 2: Bitwise AND with 0xFF (& 0xFF)
//
// Mask:    00000000 00000000 00000000 11111111
//
// +--------+--------+--------+--------+
// |   00   |   00   |   00   |   56   |  Other bits cleared, 56 remains
// +--------+--------+--------+--------+
//
// Final result: 0x56 (or 86 in decimal)

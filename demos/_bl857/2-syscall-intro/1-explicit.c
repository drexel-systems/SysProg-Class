// CONCEPT: there are a few ways to make explicit syscalls from C
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/syscall.h>

void getpid_asm();
void getpid_syscall_wrapper();

int main()
{
    // CONCEPT: this example uses a syscall function from libc
    pid_t pid = syscall(SYS_getpid);
    printf("my process ID via syscall(): %d\n", pid);

    // CONCEPT: we can also use syscall function wrappers provided by libc
    getpid_syscall_wrapper();

    // CONCEPT: and if we need explicit low level control, we can bust out the asm
    getpid_asm();

    pause(); // pause so we can look at the process id!

    return 0;
}

void getpid_asm() {
    pid_t pid;
    asm volatile(
        "mov x8, %1\n\t"  // Load syscall number (SYS_getpid) into x8
        "svc #0\n\t"      // Trigger the syscall
        "mov %0, x0"      // Store the return value (PID) from x0 into pid
        : "=r" (pid)      // Output: pid
        : "i" (SYS_getpid) // Input: syscall number for getpid
        : "x8", "x0"      // Clobbered registers
    );

    printf("my process ID via asm: %d\n", pid);
}


void getpid_syscall_wrapper()
{
    pid_t pid;
    pid = getpid();
    printf("my process ID via getpid() syscall wrapper: %d\n", pid);
}
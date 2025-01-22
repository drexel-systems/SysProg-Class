#define _GNU_SOURCE
#include <unistd.h>      // For syscall constants
#include <sys/syscall.h> // For syscall numbers (e.g., SYS_write)
#include <string.h>      // For strlen and memset
#include <sched.h>       // For clone syscall and flags
#include <sys/syscall.h> // For SYS_clone3
#include <unistd.h>      // For syscall function
#include <stdio.h>       // For printf, perror
#include <string.h>      // For memset
#include <linux/sched.h> // For struct clone_args
#include <signal.h>
#include <stdlib.h> // For malloc, free
#include <stdint.h> // for uint64_t

void simple_with_args();
void simple_with_struct();

int main()
{
    simple_with_args();
    simple_with_struct();

    return 0;
}

void simple_with_args()
{
    const char *message = "Hello, simple_with_args world!\n";
    size_t length = strlen(message);

    // Perform the syscall directly (write to stdout)
    // CONCEPT: simple argument passing
    // CONCEPT: use man pages or man7.org https://man7.org/linux/man-pages/man2/write.2.html
    ssize_t bytes_written = syscall(SYS_write, 1, message, length);

    // Check if the syscall was successful
    if (bytes_written == -1)
    {
        const char *error_msg = "Failed to write\n";
        syscall(SYS_write, 2, error_msg, 15); // Write error to stderr
    }
}

void simple_with_struct()
{
    // Define the clone_args structure (requires a modern Linux kernel)
    // CONCEPT: using structs
    // CONCEPT: struct alignment; this version uses "manual" alignment to demonstrate the concept; see below for a stdlib-ish version
    // CONCEPT: use man pages or man7.org https://man7.org/linux/man-pages/man2/clone.2.html
    struct clone_args
    {
        uint64_t flags;        // 8 bytes, no padding needed
        uint64_t pidfd;        // 8 bytes, no padding needed
        uint64_t child_tid;    // 8 bytes, no padding needed
        uint64_t parent_tid;   // 8 bytes, no padding needed
        uint64_t exit_signal;  // 8 bytes, no padding needed
        uint64_t stack;        // 8 bytes, no padding needed
        uint64_t stack_size;   // 8 bytes, no padding needed
        uint64_t tls;          // 8 bytes, no padding needed
        uint64_t set_tid;      // 8 bytes, no padding needed
        uint64_t set_tid_size; // 8 bytes, no padding needed

        // Padding: Add reserved bytes to ensure proper alignment
        uint8_t reserved[64 - (10 * 8) > 0 ? 64 - (10 * 8) : 1]; // Add extra padding to make the structure size a multiple of 64 bytes
    };

    // CONCEPT: use macros to calculate the padding for us
    // struct clone_args
    // {
    //     __aligned_u64 flags;        // Clone flags (e.g., CLONE_VM, CLONE_FS, etc.)
    //     __aligned_u64 pidfd;        // Where to store the PID file descriptor
    //     __aligned_u64 child_tid;    // Where to store child thread ID
    //     __aligned_u64 parent_tid;   // Where to store parent thread ID
    //     __aligned_u64 exit_signal;  // Signal to send on exit
    //     __aligned_u64 stack;        // Start of the stack for the child
    //     __aligned_u64 stack_size;   // Size of the stack
    //     __aligned_u64 tls;          // TLS for the child
    //     __aligned_u64 set_tid;      // Pointer to an array of TIDs
    //     __aligned_u64 set_tid_size; // Number of TIDs in the array
    // };

    struct clone_args args;
    memset(&args, 0, sizeof(args)); // Zero-initialize the struct

    args.flags = CLONE_VM | CLONE_FS; // Share memory and file system
    args.exit_signal = SIGCHLD;       // Signal sent when the child exits

    // Allocate stack for the child
    size_t stack_size = 65536; // 64 KB stack
    void *stack = malloc(stack_size);
    if (!stack)
    {
        perror("Failed to allocate stack");
        _exit(-1);
    }

    // Set stack pointer to the top (stacks grow downward)
    args.stack = (__aligned_u64)(stack + stack_size);
    args.stack_size = stack_size;

    // Call clone syscall
    int result = syscall(SYS_clone3, &args, sizeof(args));

    if (result == -1)
    {
        perror("syscall clone3 failed");
        free(stack);
        _exit(-1);
    }

    if (result == 0)
    {
        // This is the child process
        printf("Hello simple_with_struct world!\n");
        _exit(0); // Exit the child process
    }
    else
    {
        // This is the parent process
        printf("Child process created with PID: %d\n", result);
    }

    // Free the allocated stack (parent process)
    free(stack);
}

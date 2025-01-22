#include <unistd.h>

int main() {
    // const char *message = "Hello, world!\n";

    // asm volatile(
    //     "mov x8, %0\n\t"   // Syscall number for write (SYS_write = 64)
    //     "mov x0, %1\n\t"   // File descriptor (stdout = 1)
    //     "mov x1, %2\n\t"   // Pointer to message
    //     "mov x2, %3\n\t"   // Length of message
    //     "svc #0\n\t"       // Make the syscall
    //     :
    //     : "i" (SYS_write), // Input: syscall number
    //       "r" (STDOUT_FILENO), // Input: file descriptor
    //       "r" (message),       // Input: pointer to message
    //       "r" (14)             // Input: length of message
    //     : "x0", "x1", "x2", "x8" // Clobbered registers
    // );

    return 0;
}

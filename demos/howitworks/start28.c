//compile with: gcc -nostartfiles -o start28 start28.c

long syscall_error(long err) {
    // Negate the error value to get positive errno
    return -err;
}

#ifdef __aarch64__
//From https://github.com/bminor/glibc/blob/master/sysdeps/unix/sysv/linux/aarch64/syscall.S
long my_better_systemcall(long number, long a1, long a2, long a3, 
             long a4, long a5, long a6) {
    register long syscall_num __asm__("x8") = number;
    register long arg0 __asm__("x0") = a1;
    register long arg1 __asm__("x1") = a2;
    register long arg2 __asm__("x2") = a3;
    register long arg3 __asm__("x3") = a4;
    register long arg4 __asm__("x4") = a5;
    register long arg5 __asm__("x5") = a6;

    __asm__ volatile (
        "svc 0x0        \n\t"   //execute syscall to OS
        "cmn x0, #4095  \n\t"   //result in X0-4095 (negative value)
        "b.cs 1f        \n\t"   //branch if error
        "ret            \n"     //return
        "1:             \n\t"
        "b syscall_error"       //call error function in C
        : "+r" (arg0)
        : "r" (syscall_num), "r" (arg1), "r" (arg2), 
          "r" (arg3), "r" (arg4), "r" (arg5)
        : "memory"
    );

    return arg0;
}
#endif

void _start(){
    int i;
    i = 28;
    my_better_systemcall(93,i,0,0,0,0,0);
}

#if defined(__x86_64__) || defined(__amd64__) || defined(_M_X64)
//Intel/AMD 64 bit version
//
long my_better_systemcall(long syscall_number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6) {
    long result;
    asm volatile (
        "movq %1, %%rax\n\t"       // Syscall number -> rax
        "movq %2, %%rdi\n\t"       // arg1 -> rdi
        "movq %3, %%rsi\n\t"       // arg2 -> rsi
        "movq %4, %%rdx\n\t"       // arg3 -> rdx
        "movq %5, %%r10\n\t"       // arg4 -> r10
        "movq %6, %%r8\n\t"        // arg5 -> r8
        "movq %7, %%r9\n\t"        // arg6 -> r9
        "syscall\n\t"              // Perform the system call
        "cmpq $-4095, %%rax\n\t"   // Check for error (return value < -4095)
        "jae 1f\n\t"               // Jump to label 1 if error
        "1:\n\t"
        : "=a" (result)            // Output: result in rax
        : "r" (syscall_number),    // Input: syscall number
          "r" (arg1),              // Input: argument 1
          "r" (arg2),              // Input: argument 2
          "r" (arg3),              // Input: argument 3
          "r" (arg4),              // Input: argument 4
          "r" (arg5),              // Input: argument 5
          "r" (arg6)               // Input: argument 6
        : "memory", "rdi", "rsi", "rdx", "r10", "r8", "r9" // Clobbered registers
    );
    return result;
}
#endif
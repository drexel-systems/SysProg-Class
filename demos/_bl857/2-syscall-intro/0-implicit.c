// CONCEPT: implicit syscalls - how many are made to run this program?
// CONCEPT: this program does not perform any explicit syscalls
// CONCEPT: the syscalls that strace shows are from the shell/OS to load and run the program
// make && ./0-implicit
// echo $?
// strace ./0-implicit > 0-implicit-trace.txt 2>&1
int main()
{
    return 101;
}

// CONCEPT: _start - objdump -d 0-implicit > 0-implicit.txt
// CONCEPT: ASM!!!
// gcc -nostdlib -o 0-implicit 0-implicit.c
// void _start()
// {
//     // Return exit code directly using a syscall
//     __asm__(
//         "mov x8, #93\n"  // Syscall number for exit on ARM64
//         "mov x0, #102\n" // Exit code 102
//         "svc #0\n"       // Make syscall
//     );
// }
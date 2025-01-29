//compile:  gcc -nostartfiles -o start25 start25.c 
//    run:  ./start26; echo $?
//should produce 26 in shell


//Discussion points, this program starts but never returns
//lets discuss:
//   1. What is syscall() from unistd.h
//   2. Why do we need sys/syscall.h (architecture dependant)

#include <unistd.h>
#include <sys/syscall.h>

void _start() {
    syscall(SYS_exit, 26);
}
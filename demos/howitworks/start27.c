//compile:  gcc -nostartfiles -o start27 start27.c 
//    run:  ./start26; echo $?
//should produce 26 in shell


//Discussion points, this program starts but never returns
//lets discuss:
//   1. What is syscall() from unistd.h
//   2. Why do we need sys/syscall.h (architecture dependant)


#define SYS_exit 93 //ARM-64 System Call NUMBER
                    //Note its 60 on X64
                    //man syscall
                    //
                    //sudo apt install auditd
                    //ausyscall --dump

extern long int syscall (long int __sysno, ...); 

void _start() {
    syscall(SYS_exit, 27);
}
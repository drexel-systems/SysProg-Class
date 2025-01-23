#include <stdio.h>
#include <stdlib.h>

#define STACK_SZ (1024*4)

char _mystack[STACK_SZ];

const long x0 = 0;
long x1;
long x2 = (unsigned long)((void *)&_mystack + STACK_SZ); //top of stack
long x3, x4, x5, x6, x7, x8, x9, x10;
long x11, x12, x13, x14, x15, x16, x17, x18, x19, x20;
long x21, x22, x23, x24, x25, x26, x27, x28, x29, x30;
long x31;

//define API (user friendly names)
#define zero x0
#define ra x1
#define sp x2
#define gp x3
#define tp x4
#define t0 x5
#define t1 x6
#define t2 x7
#define s0 x8
#define fp x8
#define s1 x9
#define a0 x10
#define a1 x11
#define a2 x12
#define a3 x13
#define a4 x14
#define a5 x15
#define a6 x16
#define a7 x17
#define s2 x18
#define s3 x19
#define s4 x20
#define s5 x21
#define s6 x22
#define s7 x23
#define s8 x24
#define s9 x25
#define s10 x26
#define s11 x27
#define t3 x28
#define t4 x29
#define t5 x30
#define t6 x31

//Imagine writing a program where you cant create anything
//but new global variables, refer to all global memory
//by pointers, also you cant create any local variables,
//nor create any new functions

//syscalls
#define SC_PRINT_NUM    1
#define SC_PRINT_STR    4
#define SC_EXIT         10
void syscall(void);


//new global variables
long a = 5;
long b = 10;
long c = 0;
char *prompt = "The value of c is ";
char *nl     = "\n";

int main(){
    //what would you do here?
    t1 = (long)&a;
    t1 = *(long *)t1;

    t2 = (long)&b;
    t2 = *(long *)t2;

    t3 = t1 + t2;

    t4 = (long)&c;
    *(long *)t4 = t3;

    //now lets do output
    a0 = SC_PRINT_STR;
    a1 = (long)prompt;
    syscall();

    a0 = SC_PRINT_NUM;
    a1 = (long)&c;
    a1 = *(long *)a1;
    syscall();

    a0 = SC_PRINT_STR;
    a1 = (long)nl;
    syscall();
}

void syscall(void){
    switch(a0){
        case SC_PRINT_NUM:
            printf("%ld", a1);
            break;
        case SC_PRINT_STR:
            printf("%s", (char *)a1);
            break;
        case SC_EXIT:
            exit((int)a1);
            break;
    }
}




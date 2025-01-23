#include <stdio.h>

//compile gcc -o add1 add1.c


//using global variables for this demo
int a = 5;
int b = 10;
int c = 0;

int main(){
    c = a+b;
    printf("The value of c is %d\n",c);
}

/*
    What can the CPU actually do:

    1. Fetch an instruction from memory
    2. Executes the instruction
    3. Goes to 1

    Dig into #2:
        +, -, *, ....
        read from memory
        write to memory
        move things between internal registers
        branching it can do
*/
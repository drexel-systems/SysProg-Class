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
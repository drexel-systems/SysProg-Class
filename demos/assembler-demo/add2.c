#include <stdio.h>

//compile gcc -o add2 add2.c

//ref: compiler explorer: https://godbolt.org/
//using local variables (and stack) for this demo
int main(){
    int a = 5;
    int b = 10;
    int c = 0;
    c = a+b;
    printf("The value of c is %d\n",c);
}
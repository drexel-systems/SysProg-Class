#include <stdio.h>

//compile gcc -o cond cond.c

int main(){
    int a = 5;
    int b = 10;
    
    if (a < b)
        printf("The value of a < b\n");
    else
        printf("The value of a >= b\n");
}
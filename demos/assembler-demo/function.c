#include <stdio.h>

//compile gcc -o function function.c


int add_fn(int a, int b){
    return a+b;
}

int main(){
    int c = 0;
    c = add_fn(5,10);
    printf("The value of c is %d\n",c);
}
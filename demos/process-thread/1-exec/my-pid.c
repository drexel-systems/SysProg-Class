#include <stdio.h>
#include <unistd.h>

//stand alone program to just print the process id of this 
//program when it runs
int main() {
    printf("my-pid: My process id is: %d\n", getpid());
    return 0;
}
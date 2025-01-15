#include <unistd.h>
#include <stdio.h>

int main(){
    int rc = write(STDOUT_FILENO, "Hello there\n", 12);
    printf("write rc = %d\n",rc);
}